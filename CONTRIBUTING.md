# Guia de Contribuição (Para Iniciantes)

Bem-vindo(a)! Ficamos muito felizes que você queira contribuir com a `mux-sensors-lib`. Este repositório é um ótimo lugar para fazer sua primeira contribuição open-source em C!

Abaixo você encontra um guia passo-a-passo ensinando todo o fluxo de trabalho.

## Passo 1: Fork e Clone

1. No canto superior direito desta página no GitHub, clique no botão **Fork**. Isso vai criar uma cópia do repositório na sua conta.
2. Na sua cópia (Fork), clique no botão verde **Code** e copie o endereço (URL).
3. Abra o terminal no seu computador e faça o clone usando o comando abaixo:
   ```bash
   git clone <COLE-A-URL-AQUI>
   ```
4. Entre na pasta do projeto:
   ```bash
   cd mux-sensors-lib
   ```

## Passo 2: Criando uma Branch

Nunca trabalhe na branch `main`! Crie uma branch específica para o que você vai fazer:

```bash
git checkout -b feature/nome-da-sua-tarefa
```
*Exemplo: `git checkout -b feature/testes-do-sensor`*

## Passo 3: Escrevendo Código e Testando

Agora você pode editar os arquivos no seu editor de código (como o VS Code).

- **Quer rodar os testes?**
  Nós temos testes nativos em C que rodam no computador (sem precisar do Arduino). Basta abrir o terminal na pasta do projeto e digitar:
  ```bash
  make test
  ```
  Isso vai compilar os arquivos e mostrar se os testes passaram ou falharam. Se for contribuir com novas funcionalidades, lembre-se de adicionar testes no arquivo `src/test/test.c`.

## Passo 4: Formatando o Código (Importante!)

O GitHub vai barrar o seu código se ele estiver fora do padrão de formatação. Nós usamos uma ferramenta chamada `clang-format` para deixar o código bonito.

1. Instale o clang-format no seu Linux ou no Windows (via MSYS2/WSL).
   *No Linux/Ubuntu, use: `sudo apt-get install clang-format`*
2. Rode o seguinte comando no terminal (na pasta raiz do projeto) para formatar tudo automaticamente:
   ```bash
   find . -name '*.c' -o -name '*.h' -o -name '*.ino' | xargs clang-format -i
   ```

## Passo 5: Fazendo Commit e Push

1. Veja os arquivos que você modificou:
   ```bash
   git status
   ```
2. Adicione os arquivos:
   ```bash
   git add .
   ```
3. Crie um commit com uma mensagem curta e clara:
   ```bash
   git commit -m "feat: adiciona novos testes para o sensor"
   ```
4. Envie o seu código para o seu Fork no GitHub:
   ```bash
   git push origin feature/nome-da-sua-tarefa
   ```

## Passo 6: Abrindo o Pull Request (PR)

1. Vá até o seu repositório no GitHub (o seu Fork).
2. Você verá um aviso em amarelo dizendo "Compare & pull request". Clique nesse botão.
3. Preencha o template explicando o que você fez. Não esqueça de marcar o checklist com um `x`!
4. Clique em **Create pull request**.

Pronto! Agora é só esperar algum membro do projeto revisar o seu código. Se pedirmos alterações, basta alterar o código localmente, fazer commit e push novamente na mesma branch.

## Ajuda

Se você travar em qualquer passo, não tenha vergonha! Você pode abrir uma **Issue** relatando a sua dificuldade, e nós ajudaremos.
