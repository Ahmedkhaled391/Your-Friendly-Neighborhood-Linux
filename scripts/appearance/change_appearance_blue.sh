#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-blue-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#0f1320' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#e7ecff' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#1c2335', '#ff6b6b', '#78c27a', '#e6c076', '#5b8dff', '#c792ea', '#66d9e8', '#dfe6ff', '#5d6b8a', '#ff8080', '#8bd08c', '#f2cd8a', '#75a2ff', '#d5a7f3', '#80e5f2', '#ffffff']" 2>/dev/null || true
fi
