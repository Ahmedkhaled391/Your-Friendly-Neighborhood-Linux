#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-viridian-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#101a13' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#e6f2e9' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#1f2e24', '#f26d6d', '#66bb6a', '#d3b15e', '#5b8dff', '#b184f0', '#5ac8b8', '#d6e7da', '#5e7b66', '#ff8484', '#7fd184', '#e0c576', '#76a4ff', '#c69af3', '#73d8ca', '#ffffff']" 2>/dev/null || true
fi
