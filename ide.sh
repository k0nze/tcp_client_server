#!/bin/sh

SESSION_NAME=ide

if [ $(tmux ls 2>&1 | grep ${SESSION_NAME} | wc -l) -eq 0 ]; then
    tmux -2 new -s ${SESSION_NAME}\; send-keys 'vim' C-m\; split-window -v -p 20\; send-keys 'cd build' C-m\; select-pane -t 0
    else
    tmux a -t ${SESSION_NAME}
fi



