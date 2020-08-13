#!/bin/bash

sudo socat PTY,link=/dev/ttyS11 PTY,link=/dev/ttyS14
sudo ﻿socat PTY,link=/dev/ttyS12 PTY,link=/dev/ttyS15
sudo socat PTY,link=/dev/ttyS13 PTY,link=/dev/ttyS16
