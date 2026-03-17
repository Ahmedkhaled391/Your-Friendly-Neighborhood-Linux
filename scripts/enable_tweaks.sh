#!/bin/bash
sysctl -w vm.swappiness=10 && sysctl -w vm.vfs_cache_pressure=50