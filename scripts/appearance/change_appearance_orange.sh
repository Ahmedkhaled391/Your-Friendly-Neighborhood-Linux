#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-red-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#1a130f' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#f3e7de' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#2d241d', '#ff7a66', '#77c26a', '#e3b65f', '#5f8eff', '#c08bf4', '#66cfd9', '#eaded4', '#7e695a', '#ff927f', '#8fd181', '#f0c677', '#79a5ff', '#cea2f7', '#80dde6', '#ffffff']" 2>/dev/null || true
fi
