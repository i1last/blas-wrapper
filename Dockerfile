FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
    build-essential \
    wget \
    gnupg \
    ca-certificates \
    software-properties-common \
    cmake \
    make \
    git \
    bash-completion \
    vim \
    && rm -rf /var/lib/apt/lists/*

# --- Установка Intel oneAPI Base & HPC Toolkits через APT репозиторий ---
# 1. Получаем ключ Intel
RUN wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
    | gpg --dearmor | tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

# 2. Добавляем репозиторий Intel
RUN echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" \
    | tee /etc/apt/sources.list.d/oneAPI.list

# 3. Обновляем список пакетов снова
RUN apt-get update

# 4. Устанавливаем нужные компоненты oneAPI
RUN apt-get install -y --no-install-recommends \
    intel-oneapi-compiler-dpcpp-cpp \
    intel-oneapi-compiler-fortran \
    intel-oneapi-mkl-devel \
    intel-oneapi-tbb-devel \
    && rm -rf /var/lib/apt/lists/*


# --- Настройка среды ---

# Путь к скрипту инициализации переменных среды Intel oneAPI
ENV INTEL_ONEAPI_PATH=/opt/intel/oneapi

# Создаем рабочего пользователя, чтобы не работать под root
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd --gid $GROUP_ID developer && \
    useradd --uid $USER_ID --gid $GROUP_ID --create-home --shell /bin/bash developer
    
# Добавляем инициализацию oneAPI в .bashrc пользователя developer
RUN echo 'if [ -f "/opt/intel/oneapi/setvars.sh" ]; then source /opt/intel/oneapi/setvars.sh --force > /dev/null 2>&1; fi' >> /home/developer/.bashrc
RUN chown developer:developer /home/developer/.bashrc

# Создаем и назначаем владельцем рабочую директорию
RUN mkdir -p /app && chown developer:developer /app

# Создаем скрипт точки входа, который будет запускать setvars.sh
# Это гарантирует, что переменные среды Intel установлены перед выполнением любой команды
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

# Переключаемся на рабочего пользователя
USER developer

# Создаем и назначаем владельцем рабочую директорию
WORKDIR /app

# Устанавливаем точку входа
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]

# Команда по умолчанию при запуске контейнера (просто запускает bash)
CMD ["bash"]