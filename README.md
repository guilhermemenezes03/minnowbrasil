# CS144 Self-Study Check 0

Este projeto replica, de forma autoral e simplificada, as ideias centrais do `check0` do CS144:

- usar um socket TCP como fluxo confiavel de bytes;
- escrever um cliente HTTP minimo (`webget`);
- implementar um `ByteStream` em memoria com controle de capacidade;
- praticar um estilo de C++ moderno com RAII, baixo acoplamento e testes pequenos.

Nao e o repositorio oficial do curso, mas foi montado para te ensinar os mesmos conceitos.

## O que voce deve aprender aqui

1. O sistema operacional fornece uma abstracao chamada `stream socket`.
2. Para a aplicacao, esse socket parece um descritor de arquivo: voce escreve bytes e le bytes.
3. Por baixo, a rede entrega apenas datagramas com "best effort".
4. O TCP, implementado pelo sistema operacional, transforma isso num fluxo confiavel e ordenado.
5. Um `ByteStream` em memoria ajuda a entender a mesma abstracao sem envolver rede real.

## Estrutura do projeto

- `util/`: wrappers RAII para descritor de arquivo, endereco de rede e socket TCP.
- `apps/webget.cc`: exercicio de cliente HTTP usando socket TCP.
- `src/byte_stream.hh` e `src/byte_stream.cc`: exercicio do fluxo confiavel em memoria.
- `tests/`: testes de `ByteStream` e um teste de integracao do `webget`.
- `writeups/check0.md`: template para anotar descobertas e horas gastas.

## Como usar na sua VM Linux

Dentro da VM:

```bash
cd ~/cs144
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Ou usando targets parecidos com o curso:

```bash
cmake --build build --target check0
cmake --build build --target check_webget
cmake --build build --target format
cmake --build build --target tidy
```

## Fluxo sugerido de estudo

1. Leia `util/file_descriptor.hh`, `util/address.hh` e `util/socket.hh`.
2. Abra `apps/webget.cc` e implemente `get_URL`.
3. Rode:

```bash
./build/webget example.com /
```

4. Abra `src/byte_stream.hh` e `src/byte_stream.cc`.
5. Implemente `push`, `peek` e `pop`, respeitando a capacidade.
6. Rode:

```bash
cmake --build build --target check0
```

## Mapeamento com o lab original

- "4.1 setup do repositorio": aqui voce pode usar `git init`, criar um repo privado seu e adicionar um remoto `github`.
- "4.2 compiling the starter code": equivale a `cmake -S . -B build` e `cmake --build build`.
- "4.3 modern C++": siga as mesmas regras de estilo listadas no enunciado.
- "4.4 reading support code": leia os wrappers em `util/`.
- "4.5 writing webget": implemente `get_URL`.
- "5 in-memory reliable byte stream": implemente `ByteStream`.

## Git recomendado

Se quiser estudar com o mesmo ritual do curso:

```bash
git init
git add .
git commit -m "Bootstrap self-study check0 project"
```

Depois, crie um repositorio privado seu no GitHub e adicione:

```bash
git remote add github https://github.com/SEU_USUARIO/cs144-selfstudy.git
git push -u github main
```

## Observacoes

- O `webget` deste projeto aceita um terceiro argumento opcional: a porta TCP.
- Isso foi adicionado para facilitar testes locais, por exemplo em `127.0.0.1:18080`.
- Os wrappers de `util/` ja estao prontos para que voce foque nos conceitos do lab.
- Os testes do `ByteStream` e do `webget` vao falhar ate que voce implemente os TODOs.

