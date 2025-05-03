#!/bin/bash
# entrypoint.sh

# Источник переменных среды Intel oneAPI
if [ -f "/opt/intel/oneapi/setvars.sh" ]; then
    source /opt/intel/oneapi/setvars.sh --force > /dev/null
    echo "Intel oneAPI environment initialized."
else
    echo "Warning: /opt/intel/oneapi/setvars.sh not found."
fi

# Выполнить команду, переданную в docker run/compose
# `exec` заменяет процесс entrypoint процессом команды, что является хорошей практикой
exec "$@"