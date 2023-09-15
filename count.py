import os
from pathlib import Path

PATH = './src'

file_count = sum(len(files) for _, _, files in os.walk(PATH))
print(f'Total file count: {file_count} source files')

print(list(Path(PATH).rglob("*.[cC][+p][+p]")))