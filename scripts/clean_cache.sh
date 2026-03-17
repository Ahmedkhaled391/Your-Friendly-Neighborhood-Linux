#!/bin/bash
apt-get clean && journalctl --vacuum-time=7d && rm -rf /tmp/*