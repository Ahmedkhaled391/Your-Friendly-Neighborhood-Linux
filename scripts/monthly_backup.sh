#!/bin/bash
# 
#  monthly_backup.sh — Personal files backup for Ubuntu
#  Backs up: Desktop, Pictures, Downloads, Documents, Music,
#            Videos, and optionally your home config dotfiles
# 

# ── Config ──────────────────────────────────────────────────
BACKUP_ROOT="${HOME}/Backups"
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M")
BACKUP_DIR="${BACKUP_ROOT}/backup_${TIMESTAMP}"
LOG_FILE="${BACKUP_ROOT}/backup.log"
KEEP_LAST=6

SCRIPT_PATH="$(readlink -f "$0")"
CRON_TAG="YFNL_SIMPLE_BACKUP=1"

SOURCES=(
    "$HOME/Desktop"
    "$HOME/Pictures"
    "$HOME/Downloads"
    "$HOME/Documents"
    "$HOME/Music"
    "$HOME/Videos"
)

DOTFILES=(
    "$HOME/.bashrc"
    "$HOME/.zshrc"
    "$HOME/.profile"
    "$HOME/.config/gtk-3.0"
    "$HOME/.config/gtk-4.0"
    "$HOME/.ssh"
)


log() {
    mkdir -p "$BACKUP_ROOT"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*" >> "$LOG_FILE"
}

notify() {
    if command -v notify-send &>/dev/null; then
        notify-send --icon=drive-harddisk "Backup" "$1" 2>/dev/null || true
    fi
}

install_auto() {
    if ! command -v crontab >/dev/null 2>&1; then
        echo "crontab is not installed. Run: scripts/install_cron.sh" >&2
        exit 1
    fi
    (crontab -l 2>/dev/null | sed '/YFNL_SIMPLE_BACKUP=1/d' || true; \
        echo "@monthly $CRON_TAG $SCRIPT_PATH run") | crontab -
}

remove_auto() {
    if ! command -v crontab >/dev/null 2>&1; then
        echo "crontab is not installed." >&2
        exit 1
    fi
    (crontab -l 2>/dev/null | sed '/YFNL_SIMPLE_BACKUP=1/d' || true) | crontab -
}

show_status() {
    echo "Monthly cron entry:"
    crontab -l 2>/dev/null | grep -F "$CRON_TAG" || echo "Not installed"
}

case "${1:-run}" in
install-auto)
    install_auto
    exit 0
    ;;
remove-auto)
    remove_auto
    exit 0
    ;;
status)
    show_status
    exit 0
    ;;
run)
    ;;
*)
    echo "Unknown command: ${1:-}" >&2
    echo "Use: run | install-auto | remove-auto | status" >&2
    exit 1
    ;;
esac

log "========================================"
log "Backup started → ${BACKUP_DIR}"

ERRORS=0


for SRC in "${SOURCES[@]}"; do
    if [ -d "$SRC" ]; then
        DEST="${BACKUP_DIR}/$(basename "$SRC")"
        mkdir -p "$DEST"
        rsync -aq "$SRC/" "$DEST/" >>"$LOG_FILE" 2>>"$LOG_FILE"
        if [ $? -ne 0 ]; then
            log "WARNING: rsync reported errors for $SRC"
            ((ERRORS++))
        fi
    fi
done

# Back up dotfiles
DOTFILES_DEST="${BACKUP_DIR}/dotfiles"
mkdir -p "$DOTFILES_DEST"
for DOT in "${DOTFILES[@]}"; do
    if [ -e "$DOT" ]; then
        rsync -aq "$DOT" "$DOTFILES_DEST/" >>"$LOG_FILE" 2>>"$LOG_FILE"
    fi
done


ARCHIVE="${BACKUP_ROOT}/backup_${TIMESTAMP}.tar.gz"
tar -czf "$ARCHIVE" -C "$BACKUP_ROOT" "backup_${TIMESTAMP}" 2>>"$LOG_FILE"

if [ $? -eq 0 ]; then
    rm -rf "$BACKUP_DIR"
    ARCHIVE_SIZE=$(du -sh "$ARCHIVE" | cut -f1)
    log "Archive created: $ARCHIVE ($ARCHIVE_SIZE)"
else
    log "ERROR: Compression failed. Uncompressed backup kept at $BACKUP_DIR"
    ((ERRORS++))
    ARCHIVE_SIZE="unknown"
fi


ls -1t "${BACKUP_ROOT}"/backup_*.tar.gz 2>/dev/null | tail -n +$((KEEP_LAST + 1)) | while read -r OLD; do
    log "Removing old backup: $OLD"
    rm -f "$OLD"
done


if [ "$ERRORS" -eq 0 ]; then
    MSG="Backup completed successfully ($ARCHIVE_SIZE)"
    log "$MSG"
    notify "$MSG"
else
    MSG="Backup finished with $ERRORS error(s). Check $LOG_FILE"
    log "$MSG"
    notify "$MSG"
fi

log "========================================"
exit $ERRORS
