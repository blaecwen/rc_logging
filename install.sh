#!/bin/bash

# install.sh - a script to install distributed robotic log delivery system


function install {
    # set -x
    set -e
    echo "Installing rclogd..."

    adduser --system rclog --group --no-create-home

    mkdir -p /etc/rclog
    cp rclogd.cfg /etc/rclog
    chown -R rclog:rclog /etc/rclog
    chmod -R g+rw /etc/rclog

    echo "Copying files..."
    cp build-rclogd/rclogd /usr/local/bin
    cp rclogd.service /etc/systemd/system
    systemctl enable rclogd.service
    systemctl start rclogd.service
    echo "Installed rclogd successfully"
}

function uninstall {
    # set -x
    echo "Uninstalling rclogd..."

    systemctl stop rclogd.service

    rm -rf /etc/rclog
    rm /usr/local/bin/rclogd
    rm /etc/systemd/system/rclogd.service

    deluser --system rclog 
    delgroup --system rclog -q

    systemctl daemon-reload
    echo "Uninstalled rclogd successfully"
}


if [ "$EUID" -ne 0 ]
    then echo "Please run as root"
    exit 1
fi

if [ $# -ne "1" ]; then
    echo "Error: $0 should be started with task name in first argument (e.g. \"$0 install\")"
    exit 1
fi




if [ "$1" == "install" ]; then
    install
elif [ "$1" == "uninstall" ]; then
    uninstall   
else
    echo "Error: unknown task: \"$1\""
    exit 1
fi
