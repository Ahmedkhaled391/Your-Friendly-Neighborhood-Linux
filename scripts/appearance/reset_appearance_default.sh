#!/usr/bin/env bash

set -euo pipefail

gsettings reset org.gnome.desktop.interface gtk-theme
gsettings reset org.gnome.desktop.interface icon-theme
gsettings reset org.gnome.desktop.interface color-scheme 2>/dev/null || true
gsettings reset org.gnome.Terminal.Legacy.Settings theme-variant 2>/dev/null || true

profile_id="$(gsettings get org.gnome.Terminal.ProfilesList default 2>/dev/null | tr -d "'")"
if [[ -n "${profile_id:-}" ]]; then
	profile_schema="org.gnome.Terminal.Legacy.Profile:/org/gnome/terminal/legacy/profiles:/:${profile_id}/"
	gsettings reset "$profile_schema" use-theme-colors 2>/dev/null || true
	gsettings reset "$profile_schema" use-theme-transparency 2>/dev/null || true
	gsettings reset "$profile_schema" background-color 2>/dev/null || true
	gsettings reset "$profile_schema" foreground-color 2>/dev/null || true
	gsettings reset "$profile_schema" bold-color-same-as-fg 2>/dev/null || true
	gsettings reset "$profile_schema" palette 2>/dev/null || true
fi