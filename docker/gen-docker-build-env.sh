#!/bin/sh

set -e

COMPILER_VERSIONS="clang-6.0 g++-7"
SECRET_BACKENDS="qt5keychain-dev libkf5wallet-dev qt5keychain-dev;libkf5wallet-dev"

DOCKER_NAMESPACE="cmacq2/factorkey-build-env"
DOCKER_BASE_IMAGE_TAG="base"

push_files=""

append_cmd_arg ()
{
    if [ -n "$1" ]
    then
        echo -n "$1, \"$2\""
    else
        echo -n "\"$2\""
    fi
}

get_cmd_string ()
{
    result=""
    if echo -n "$@" | grep -q qt5keychain
    then
        result="$(append_cmd_arg "$result" -DWITH_QT_KEYCHAIN=1)"
    else
        result="$(append_cmd_arg "$result" -DWITH_QT_KEYCHAIN=0)"
    fi
    if echo -n "$@" | grep -q libkf5wallet
    then
        result="$(append_cmd_arg "$result" -DWITH_KWALLET=1)"
    else
        result="$(append_cmd_arg "$result" -DWITH_KWALLET=0)"
    fi
    echo -n "$result"
}

get_env_string ()
{
    compiler=$(echo -n "$@" | cut -d' ' -f1)
    compiler_prefix=$(echo -n "$compiler" | cut -d- -f1)
    compiler_version=$(echo -n "$compiler" | cut -d- -f2)
    case "$compiler_prefix" in
        clang*)
            echo -n "CC=\"$compiler\" CXX=\"$compiler_prefix++-$compiler_version\""
        ;;
        g++*)
            echo -n "CC=\"gcc-$compiler_version\" CXX=\"$compiler_prefix-$compiler_version\""
        ;;
    esac
}

echo_docker_file ()
{
cmd_string=$(get_cmd_string "$@")
env_string=$(get_env_string "$@")
cat << EOD
#
# Auto generated docker file for build env: $@
#
FROM $DOCKER_NAMESPACE:$DOCKER_BASE_IMAGE_TAG
RUN apt-get update -qq && apt-get install -qq -y --no-install-recommends \\
    $@

CMD [$cmd_string]
ENV $env_string
EOD
}

convert_semi_to_space ()
{
    echo -n $@ | tr ';' ' '
}

build_env_to_file_name ()
{
    echo -n $@ | tr ' ' '_'
}

build_env_to_tag_name ()
{
    echo -n $@ | tr ' +' '_.'
}

gen_test_matrix ()
{
    for compiler in $COMPILER_VERSIONS
    do
        for backend in $SECRET_BACKENDS
        do
            echo $(convert_semi_to_space $compiler) $(convert_semi_to_space $backend)
        done
    done
}

iterate_test_matrix ()
{
    gen_test_matrix | while read build_env || [ -n "$build_env" ]
    do
        $1 "$2" "$build_env"
    done
}

build_base_docker_image ()
{
    docker_file_name="$1/build-env-base.docker"
    docker_tag="$DOCKER_BASE_IMAGE_TAG"
    echo "Building docker image from: '$docker_file_name' ($docker_tag)"
    docker_context="$1"
    if [ -z "$1" ]
    then
        docker_context="/"
    fi
    docker build --rm=true -t "$DOCKER_NAMESPACE:$docker_tag" -f "$docker_file_name" "$docker_context"
    if [ -n "$push_files" ]
    then
        docker push "$DOCKER_NAMESPACE:$docker_tag"
    fi
}

build_docker_image ()
{
    docker_file_name="$1/$(build_env_to_file_name "$2").docker"
    docker_tag="$(build_env_to_tag_name "$2")"
    echo "Building docker image from: '$docker_file_name' ($2)"
    docker build --rm -t "$DOCKER_NAMESPACE:$docker_tag" - <"$docker_file_name"
    if [ -n "$push_files" ]
    then
        docker push "$DOCKER_NAMESPACE:$docker_tag"
    fi
}

gen_docker_file ()
{
    docker_file_name="$1/$(build_env_to_file_name "$2").docker"
    echo "Writing output to: '$docker_file_name' ($2)"
    echo_docker_file "$2">"$docker_file_name"
}

print_docker_file ()
{
    docker_file_name="$1/$(build_env_to_file_name "$2").docker"
    echo "$docker_file_name"
}

gen_docker_usage ()
{
    cat << USAGE
$1

Usage: $(basename "$0") [options]

Options:

 -h, --help             : Display this 'help' message
 -d, --dir,       <dir> : Pass output directory <dir> in which to generate docker files
     --directory        :
 -c, --cwd              : Output docker files in the current working directory
 -b, --build            : Build previously generated docker files
 -l, --list             : List docker files, but do not actually generate them
 -p, --push             : Also push docker files to the Docker Hub repository

USAGE
    exit $2
}

case "$1" in
    -h|--help)
        gen_docker_usage "$(basename $0) : generates a test matrix of build environments for 'factorkey'" 0
    ;;
    *)
        output_dir=""
        nondefault=""
        while [ $# -ge 1 ]
        do
            case "$1" in
                -d|--dir|--directory)
                    if [ -n "$output_dir" ]
                    then
                        gen_docker_usage "Duplicate directory option: $1 $2" 1
                    elif [ -n "$2" -a -d "$2" ]
                    then
                        output_dir="$2"
                        shift 1
                    else
                        gen_docker_usage "A valid directory is required, got: '$2'" 1
                    fi
                ;;
                -b|--build)
                    if [ -n "$nondefault" ]
                    then
                        gen_docker_usage "Conflicting option: $1" 1
                    else
                        nondefault="build"
                    fi
                ;;
                -l|--list)
                    if [ -n "$nondefault" ]
                    then
                        gen_docker_usage "Conflicting option: $1" 1
                    elif [ -n "$push_files" ]
                    then
                        gen_docker_usage "Not a build command: $1" 1
                    else
                        nondefault="list"
                    fi
                ;;
                -p|--push)
                    if [ -n "$nondefault" -a "$nondefault" != "build" ]
                    then
                        gen_docker_usage "Conflicting option, must be part of a build command: $1" 1
                    else
                        push_files="yes"
                    fi
                ;;
                -c|--cwd)
                    if [ -n "$output_dir" ]
                    then
                        gen_docker_usage "Duplicate directory option: $1"
                    else
                        output_dir="$(pwd)"
                    fi
                ;;
                *)
                    gen_docker_usage "Invalid option: '$1'" 255
                ;;
            esac
            shift 1
        done

        if [ -z "$output_dir" ]
        then
            gen_docker_usage "A valid directory is required." 1
        elif [ "$output_dir" = "/" ]
        then
            output_dir=""
        fi

        case "$nondefault" in
            list)   iterate_test_matrix print_docker_file "$output_dir" ;;
            build)
                build_base_docker_image "$output_dir"
                iterate_test_matrix build_docker_image "$output_dir"
            ;;
            *)      iterate_test_matrix gen_docker_file "$output_dir"
        esac
    ;;
esac

