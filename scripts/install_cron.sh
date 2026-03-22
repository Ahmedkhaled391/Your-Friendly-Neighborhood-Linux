#!/bin/bash

if command -v crontab >/dev/null 2>&1; then
    exit 0
fi

apt-get install -y cron
systemctl enable cron
systemctl start cron
