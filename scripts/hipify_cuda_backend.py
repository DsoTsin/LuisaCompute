import json
from sys import argv
from os import path
from shutil import which
from subprocess import run


def hipify(hipify_exe, cuda_path, src, command):
    args = [hipify_exe, f"--cuda-path={cuda_path}", src]
    args.extend(f"-{x}" for x in command.split(" -")
                if x.startswith("I") or x.startswith("D") or x.startswith("U"))
    print(" ".join(args))
    run(args)


def main():
    hipify_clang = which("hipify-clang")
    print(hipify_clang)
    nvcc = which("nvcc")
    cuda_path = path.dirname(path.dirname(nvcc))
    print(cuda_path)
    with open(argv[1], 'r') as f:
        compile_commands = json.load(f)
    items = [x for x in compile_commands if
             "src/backends/cuda" in x['file'] and "/src/backends/cuda/CMakeFiles" not in x['file']]
    for item in items:
        hipify(hipify_clang, cuda_path, item['file'], item['command'])


if __name__ == '__main__':
    main()
