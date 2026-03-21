#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'default' 2>/dev/null || true
gsettings set org.gnome.Terminal.Legacy.Settings theme-variant 'light' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#ffffff' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#111318' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#111318', '#c43d43', '#3f8f4f', '#9a7b22', '#2d6cdf', '#7b4fd6', '#218a8a', '#d8dce3', '#5e646f', '#d24d4d', '#56a666', '#b08d2b', '#3f7bef', '#8f62e3', '#2f9b9b', '#ffffff']" 2>/dev/null || true
fi
