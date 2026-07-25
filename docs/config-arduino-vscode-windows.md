# Configurando o VS Code para compilar o `mux-sensors-lib-2` (Windows)

Este guia resolve dois problemas:

1. O erro `cannot open source file "Arduino.h"` no VS Code (falha do IntelliSense).
2. Fazer o VS Code **compilar de verdade** o código (o MSYS2 sozinho não faz isso).

> **Por que o MSYS2 não é suficiente:** o `pacman` do MSYS2 instala um compilador GCC genérico (mingw-w64), que serve para compilar programas para Windows. Só que compilar para um Arduino (AVR) exige um compilador diferente — o `avr-gcc` — junto com os headers do "core" do Arduino (onde mora o `Arduino.h`). Quem cuida disso é o **Arduino CLI**, não o MSYS2.

---

## Passo 1 — Instalar o Arduino CLI

1. Baixe o instalador em: https://arduino.github.io/arduino-cli/latest/installation/ (escolha a versão Windows).
2. Extraia o `arduino-cli.exe` para uma pasta fixa, por exemplo `C:\arduino-cli\`.
3. Adicione essa pasta ao PATH do Windows:
   - Pesquise "Variáveis de Ambiente" no menu Iniciar → **Editar as variáveis de ambiente do sistema**.
   - Em **Variáveis do usuário**, edite `Path` → **Novo** → cole `C:\arduino-cli\`.
   - Feche e reabra qualquer terminal para o PATH atualizar.
4. Teste no PowerShell:
   ```powershell
   arduino-cli version
   ```
   Se aparecer o número da versão, deu certo.

## Passo 2 — Instalar o core do Arduino (AVR)

No PowerShell:

```powershell
arduino-cli core update-index
arduino-cli core install arduino:avr
```

Isso baixa o compilador `avr-gcc` e os headers (`Arduino.h`, `pins_arduino.h`, etc.) para dentro do Arduino CLI — é isso que resolve o erro de importação.

Para descobrir exatamente onde esses arquivos ficaram (você vai precisar do caminho no Passo 4):

```powershell
arduino-cli config dump
```

Procure a linha `directories: data:` — o caminho normalmente é:

```
C:\Users\<SEU_USUARIO>\AppData\Local\Arduino15
```

Dentro dela, os headers do core ficam em:

```
C:\Users\<SEU_USUARIO>\AppData\Local\Arduino15\packages\arduino\hardware\avr\<versão>\cores\arduino
C:\Users\<SEU_USUARIO>\AppData\Local\Arduino15\packages\arduino\hardware\avr\<versão>\variants\standard
```

(troque `<versão>` pelo número de pasta que aparecer aí, ex. `1.8.6`)

## Passo 3 — Organizar a pasta do projeto

O Arduino exige que **só exista um arquivo `.ino` "principal"** por pasta de compilação, e esse arquivo precisa ter **o mesmo nome da pasta**. Como o repositório tem `main.ino` **e** `test.ino` na mesma pasta, compilar direto vai dar erro de `setup()`/`loop()` duplicados.

Organize assim antes de compilar:

- Para compilar a aplicação normal: mantenha só `main.ino` na pasta raiz e **mova `test.ino` para fora** (ex. para uma subpasta `teste/` separada, com sua própria cópia de `src/`, ou apenas renomeie temporariamente para `test.ino.bak`).
- Para compilar os testes: faça o inverso.
- Renomeie o `.ino` que ficou na pasta para o **mesmo nome da pasta** do repositório (ex. se a pasta se chama `mux-sensors-lib-2`, o arquivo deve se chamar `mux-sensors-lib-2.ino`).

## Passo 4 — Configurar o VS Code

Dentro da pasta do repositório, edite (ou crie) os arquivos abaixo em `.vscode/`.

### `.vscode/c_cpp_properties.json`

```json
{
  "configurations": [
    {
      "name": "Arduino-Win32",
      "includePath": [
        "${workspaceFolder}/**",
        "C:/Users/SEU_USUARIO/AppData/Local/Arduino15/packages/arduino/hardware/avr/VERSAO/cores/arduino",
        "C:/Users/SEU_USUARIO/AppData/Local/Arduino15/packages/arduino/hardware/avr/VERSAO/variants/standard"
      ],
      "defines": ["ARDUINO=10607", "F_CPU=16000000L", "__AVR_ATmega328P__"],
      "compilerPath": "C:/Users/SEU_USUARIO/AppData/Local/Arduino15/packages/arduino/tools/avr-gcc/VERSAO/bin/avr-gcc.exe",
      "cStandard": "gnu11",
      "cppStandard": "gnu++17",
      "intelliSenseMode": "gcc-x64"
    }
  ],
  "version": 4
}
```

Substitua `SEU_USUARIO` e `VERSAO` pelos valores reais que você encontrou no Passo 2. Se o board não for um Uno/Nano (ATmega328P), ajuste `__AVR_ATmega328P__` e a pasta `variants` conforme a placa usada.

### `.vscode/tasks.json` (para compilar com um atalho)

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Compilar (Arduino Uno)",
      "type": "shell",
      "command": "arduino-cli",
      "args": ["compile", "--fqbn", "arduino:avr:uno", "${workspaceFolder}"],
      "group": { "kind": "build", "isDefault": true },
      "problemMatcher": []
    },
    {
      "label": "Gravar na placa (Arduino Uno)",
      "type": "shell",
      "command": "arduino-cli",
      "args": [
        "upload",
        "-p", "COM3",
        "--fqbn", "arduino:avr:uno",
        "${workspaceFolder}"
      ],
      "problemMatcher": []
    }
  ]
}
```

- Troque `arduino:avr:uno` pela placa correta, se não for um Uno (`arduino-cli board listall` mostra os nomes disponíveis).
- Troque `COM3` pela porta serial correta (Gerenciador de Dispositivos do Windows, em "Portas (COM e LPT)").

## Passo 5 — Testar

1. Feche e reabra a pasta do projeto no VS Code (ou `Ctrl+Shift+P` → **Developer: Reload Window**).
2. Abra `src/mux/multiplexer.h` — o sublinhado vermelho em `Arduino.h` deve sumir.
3. Aperte `Ctrl+Shift+B` para rodar a task de **Compilar**. O terminal deve mostrar o resultado da compilação (erros reais de código aparecem aqui, não mais erros de path).
4. (Opcional) Conecte a placa por USB e rode a task **Gravar na placa** para fazer upload.

## Se der errado

- **`arduino-cli` não é reconhecido**: o PATH não foi atualizado — reabra o terminal/VS Code, ou confira o Passo 1.3.
- **`Arduino.h` continua com erro**: confira se o caminho no `includePath` bate exatamente com a pasta que apareceu em `arduino-cli config dump` (a versão do core muda o nome da subpasta).
- **Erro de `setup` ou `loop` redefinidos ao compilar**: sobrou mais de um `.ino` na mesma pasta — revise o Passo 3.
- **Placa não encontrada na hora de gravar**: confira a porta COM no Gerenciador de Dispositivos e ajuste em `tasks.json`.
