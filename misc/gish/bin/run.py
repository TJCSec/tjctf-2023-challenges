#!/usr/bin/python3
import sys
import shlex
import subprocess
from os import chdir

chdir('/srv/')
print('please enter your script, terminated by the word "end"')
sys.stdout.flush()
script_lines = []

while True:
  next_line = sys.stdin.readline().strip()
  if next_line == 'end':
    break
  script_lines.append(next_line)
print('script entered')
sys.stdout.flush()
for line in script_lines:
  try:
    args = shlex.split(line)
    assert args[0] == 'git'
    output = subprocess.check_output(args, timeout=10)
    print(output.decode('utf-8'))
  except:
    print('errored, exiting')
  sys.stdout.flush()