#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-bark-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#1a1710' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#f4edd8' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#2f2a20', '#f08070', '#85c875', '#e0c066', '#6f93ff', '#ba90f2', '#74d0d0', '#e7dfc9', '#847860', '#ff9888', '#9ad88b', '#eccd7c', '#89a9ff', '#c9a6f5', '#8adedf', '#ffffff']" 2>/dev/null || true
fi