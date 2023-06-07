#!/bin/sh
main () {
  # there should only be one file with this name, so we can pass directly to cat
  cat $(find / -name 'flag-*.txt')
}

main "$@"
