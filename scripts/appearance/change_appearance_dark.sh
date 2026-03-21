#!/usr/bin/env bash

set -euo pipefail

gsettings set org.gnome.desktop.interface gtk-theme 'Yaru-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark' 2>/dev/null || true
gsettings set org.gnome.Terminal.Legacy.Settings theme-variant 'dark' 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
    profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
    gsettings set "$profile_schema" use-theme-colors false 2>/dev/null || true
    gsettings set "$profile_schema" use-theme-transparency false 2>/dev/null || true
    gsettings set "$profile_schema" background-color '#07090d' 2>/dev/null || true
    gsettings set "$profile_schema" foreground-color '#e6ebf2' 2>/dev/null || true
    gsettings set "$profile_schema" bold-color-same-as-fg true 2>/dev/null || true
    gsettings set "$profile_schema" palette "['#1f2430', '#ff6b6b', '#7bc275', '#e6c07b', '#63a3ff', '#c792ea', '#5fd7d7', '#d9dee7', '#5c6370', '#ff7f7f', '#98d38b', '#f2d08f', '#7ab0ff', '#d7a4f0', '#7de3e3', '#ffffff']" 2>/dev/null || true
fi