#!/usr/bin/env bash
if [ "$1" == "add" ]; then
    sudo tc qdisc add dev lo root netem delay $2ms
fi
if [ "$1" == "del" ]; then
    sudo tc qdisc del dev lo root
fi
