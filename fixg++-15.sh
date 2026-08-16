#!/bin/bash

set -euo pipefail

# https://github.com/orgs/Homebrew/discussions/6229#discussioncomment-14079111

brew update
brew upgrade
brew install binutils
brew reinstall binutils
sudo update-alternatives \
    --install /usr/bin/as as /home/linuxbrew/.linuxbrew/opt/binutils/bin/as 100 \
    --slave /usr/bin/ld ld /home/linuxbrew/.linuxbrew/opt/binutils/bin/ld \
    --slave /usr/bin/nm nm /home/linuxbrew/.linuxbrew/opt/binutils/bin/nm \
    --slave /usr/bin/objdump objdump /home/linuxbrew/.linuxbrew/opt/binutils/bin/objdump \
    --slave /usr/bin/objcopy objcopy /home/linuxbrew/.linuxbrew/opt/binutils/bin/objcopy \
    --slave /usr/bin/strip strip /home/linuxbrew/.linuxbrew/opt/binutils/bin/strip
sudo update-alternatives --config as