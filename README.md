# blas_wrapper

## How to build
1. Install Docker on your desktop
2. `docker compose build`
3. `docker compose up -d`
4. To entry in container use `docker compose exec build_env bash`. All files are in `/app` directory.
5. Create and go to `/app/build` (`mkdir build && cd build`)
6. `cmake ..`
7. `cmake --build .`
8. Run tests by `ctest --test-dir tests`
9. Product in `/app/build/blas_wrapper/`

## Cmake options
### Compiler verbose
Use `cmake -DCOMPILER_VERBOSE=ON ..` instead ~~`cmake ..`~~

### For debuging
Use `cmake -DTESTING_MODE=ON ..` instead ~~`cmake ..`~~

### Tests files
By default tests files always compile. To turn off this use  `cmake -DBUILD_TESTS=OFF ..` instead ~~`cmake ..`~~

## For VS Code users
If you use VS Code then configure `.vscode/launch.json` like this:
```WIP: HOW???```

<!-- Then use extension [Remote Development](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack):
1. `Ctrl + Shift + P`
2. `Remote-Containers: Attach to Running Container`
3. Select `blas_wrapper_container` -->

# WARNING: Legal & Educational Disclaimer
## EN
### ⚠️ Important Notice

This project is purely educational and was created to explore:
- Intel® oneAPI toolkits
- BLAS/LAPACK functionality
- High-performance computing concepts

Key Points:
- Non-Commercial Use – All code and examples are provided for learning purposes only, not for production deployment.
- Intel® Licensing – This project does not modify, redistribute, or reverse-engineer any Intel® proprietary components.
- Intel® oneAPI tools are used as-is under Intel’s [End User License Agreement (EULA)](https://www.intel.com/content/www/us/en/developer/articles/legal/oneapi-license.html).
- No Warranty – The author assumes no liability for accuracy, performance, or compliance with third-party licenses.
- Fair Use – Any references to Intel® trademarks or documentation fall under "fair use" for educational commentary.

For Legal Compliance:
- Consult Intel’s official licensing terms before using oneAPI components in your projects.
- Commercial users should obtain proper Intel® licenses.

## RU
### ⚠️ Внимание

Данный проект создан исключительно в образовательных целях для изучения:
- Инструментов Intel® oneAPI
- Функциональности BLAS/LAPACK
- Высокопроизводительных вычислений

Ключевые положения:
- Некоммерческое использование – Весь код и примеры предназначены только для обучения, а не для промышленного применения.
- Лицензирование Intel® – Проект не изменяет, не распространяет и не декомпилирует проприетарные компоненты Intel®.
- Инструменты Intel® oneAPI используются «как есть» в соответствии с [Лицензионным соглашением Intel (EULA)](https://www.intel.com/content/www/us/en/developer/articles/legal/oneapi-license.html).
- Отказ от гарантий – Автор не несёт ответственности за точность, производительность или соответствие сторонним лицензиям.
- Добросовестное использование – Упоминания товарных знаков Intel® и документации подпадают под «добросовестное использование» в образовательном контексте.

Для соблюдения юридических норм:
- Перед использованием компонентов oneAPI в своих проектах ознакомьтесь с официальными лицензиями Intel®.
- Коммерческим пользователям необходимо оформить соответствующие лицензии Intel®.