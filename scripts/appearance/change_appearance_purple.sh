#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-purple-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
	gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
	gsettings set "$profile_schema" background-color '#171021' 2>/dev/null || true
	gsettings set "$profile_schema" foreground-color '#ece6f6' 2>/dev/null || true
	gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
	gsettings set "$profile_schema" palette "['#292038', '#f27272', '#7ec97a', '#ddc06a', '#6b8fff', '#b48af0', '#68d0d8', '#e2d8ef', '#6f6282', '#ff8b8b', '#96d591', '#e8cc80', '#84a5ff', '#c6a1f4', '#82dce4', '#ffffff']" 2>/dev/null || true
fi
