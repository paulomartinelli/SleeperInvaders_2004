;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;MIT License
;
;Copyright (c) 2019 paulomartinelli
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;SOFTWARE.
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Universidade Federal de Pernambuco                                           x
;x Projeto de Assembly                                                          x
;x Infraestrutura de Software - Prof Sérgio Cavalcante                          x
;x Sleeper Invaders                                                             x
;x Grupo:                                                                       x
;x Ana Elizabeth Xavier Correia de Albuquerque        (aexca@cin.ufpe.br)       x
;x Carina Neiva Soares                                (cns@cin.ufpe.br)         x
;x Emanoel Manasses Bernado Xavier                    (embxcin.ufpe.br)         x
;x Leonardo Lira Rafael da Silva                      (llrs@cin.ufpe.br)        x
;x Paulo Martinelli Hemmlepp                          (pmh@cin.ufpe.br)         x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

;--------------------------------------------------------------------------------
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Proc ReadFile                                                                x
;x Copia Arquivo para Buffer                                                    x
;x                                                                              x
;x  Parametros                                                                  x
;x   DS:DX   Nome do Arquivo                                                    x
;x   ES:DI   Buffer                                                             x
;x   CX      Tamanho                                                            x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
ReadFile        PROC    NEAR

; Empilha registradores
pusha
push ds
push es

; Abre o arquivo e verifica erros
mov ax, 3D00h
int 21h
jc  ErrorOpenFile

;Copia o Handle de AX para BX
mov bx, ax

;Copia o conteudo do arquivo para o buffer
mov dx, es
mov ds, dx
mov dx, di
mov ah, 3Fh
int 21h
jc  ErrorOpenFile

;Fecha o arquivo
mov ah, 3Eh
int 21h
jc  ErrorOpenFile

;Finaliza o procedimento
RFExit:

;Desempilha registradores
pop es
pop ds
popa
ret

;Tratamento de erros
ErrorOpenFile1:

;*********************************************
;
;TRATAMENTO DE ERRO
;
;*********************************************
  mov dx, offset msg4
  mov ax, 03h                ; mudar para mostrar figura
  int 10h

  ;Exibe a string (DS:DX) na tela
  mov ah, 09h
  int 21h

  ;Retorna o controle ao SO
  mov ax, 4C01h
  int 21h
ErrorOpenFile:

  ;Verifica se foi FileNotFoundError
  cmp ax, 02h
  je  ErrorFileNotFound

  ;DS:DX aponta para mensagem de erro
  mov dx, offset msg1
  jmp ErrorExit

  ;Trata o erro de FileNotFoundError
  ErrorFileNotFound:

  ;Verifica se foi o arquivo de Recordes
  cmp dx, offset scoreFile
  je  CreateScoreFile

  ;DS:DX aponta para mensagem de erro
  mov dx, offset msg2
  ;Finaliza o tratamento de erros
ErrorExit:

  ;Retorna ao modo texto
  mov ax, 03h
  int 10h

  ;Exibe a string (DS:DX) na tela
  mov ah, 09h
  int 21h

  ;Retorna o controle ao SO
  mov ax, 4C01h
  int 21h

  ;Cria o arquivo de Recordes
CreateScoreFile:
  ;Cria o arquivo
  mov cx, 00h
  mov ah, 3Ch
  int 21h

  ;Fecha o arquivo
  mov bx, ax
  mov ah, 3Eh
  int 21h

  ;Finaliza o procedimento
  jmp RFExit

  ;Final do procedimento ReadFile
ReadFile        ENDP

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Proc DesenhaTela                                                             x
;x                                                                              x
;x Descrição: Copia uma imagem de um Arquivo para o Video                       x
;x                                                                              x
;x Parametros                                                                   x
;x   DS:DX   Nome do Arquivo (ASCIIZ)                                           x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
DesenhaTela PROC NEAR

;Empilha Registradores
push ax
push bx
push cx
push dx
push ds
push es

;Abre o Arquivo (3D) como somente leitura (00)
mov ah, 3Dh
;Seta somente leitura
mov al, 00h
int 21h

;Copia o Handle do Arquivo de AX para BX que sera usado na proxima int 21h
mov bx, ax

;DS:DX Aponta para a Memoria de Video
mov ax, 0A000h
mov ds, ax
mov dx, 0

;Lê a partir do arquivo aberto e o copia para a Memoria de Video
;cx - Numero de iterações - 64000 (320x200)
mov cx, 64000
mov ah, 3Fh
int 21h

;Fecha o Arquivo
mov ah, 3Eh
int 21h

;Desempilha Registradores
pop es
pop ds
pop dx
pop cx
pop bx
pop ax
ret
;Fim do procedimento DesenhaTela
DesenhaTela     ENDP


 ;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x PROC CarregaPaleta                                                           x
;x                                                                              x
;x Descrição: Procedimento que carrega uma paleta de cores                      x
;x            na sua variável correspondente.                                   x
;x                                                                              x
;x Parametros:                                                                  x
;x DS:DX - Nome do arquivo da paleta                                            x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
CarregaPaleta PROC NEAR

;Empilha os registradores
push ax
push bx
push cx
push dx
push ds
push es
push di
push si

;Faz com que ES aponte para o segmento de dados
mov ax,@data
mov es,ax   ;trocar depois para ds, pra ver se funciona

;Passa o endereço do arquivo da paleta para DI
mov di, offset palleteBuffer       ; trocar para dx
mov cx, 768
;Lê o arquivo
call ReadFile

;Ativa escrita da paleta (Porta 3C8H)

;Avisa a interface de video que a paleta vai ser modificada
mov dx,3C8h
mov al,0
out dx,al     ; por que avisar com zero?

;Incrementa o valor de dx para 3C9H, para modificar a paleta
inc dx

;Manda 768 bytes do buffer para a paleta
;outsb envia o conteúdo de si para o endereço da porta contido em dx
mov si, offset palleteBuffer
mov ax, seg palleteBuffer
mov ds,ax
mov cx,768
rep outsb


; Desempilha Registradores
pop si
pop di
pop es
pop ds
pop dx
pop cx
pop bx
pop ax
ret

;Fim do procedimento Carregar Paleta
CarregaPaleta ENDP

;*************************************************
;PROCEDIMENTO QUE LIMPA A TELA E MUDA PARA O MODO TEXTO
;*************************************************
LimpaTela PROC NEAR
push es di cx ax
;********Coloca no modo texto (85X20)
mov ax, 03h
int 10h

;********limpa a tela***************************
mov   ax, 0A000h     ; Move o endereço de vídeo para AX
mov   es, ax        ; Aponta ES para o segmento de vídeo
xor   di, di        ; Zera  DI
mov   cx, 2000     ; Move 32000 (320x200) para CX
mov   ah, 00h       ; Move o atributo para AH
mov   al, 00h       ; Move o caracter a usar para AL
rep   stosw
pop ax cx di es
ret
LimpaTela ENDP

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x PROC CarregaPaleta2                                                           x
;x                                                                              x
;x Descrição: Procedimento que carrega uma paleta de cores                      x
;x            na sua variável correspondente.                                   x
;x                                                                              x
;x Parametros:                                                                  x
;x DS:SI - Nome do arquivo da paleta                                            x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
CarregaPaleta2 PROC NEAR ;MACRO OffsetPaleta

  push ax dx cx
  ;Ativa escrita da paleta (Porta 3C8H)
 ; mov si, OffsetPaleta
  ;Avisa a interface de video que a paleta vai ser modificada
  mov dx,3C8h
  mov al,0
  out dx,al

  ;Incrementa o valor de dx para 3C9H, para modificar a paleta
  inc dx

  ;Manda 768 bytes do buffer para a paleta
  ;outsb envia o conteúdo de si para o endereço da porta contido em dx
  mov cx,768
  rep outsb
  pop cx dx ax
  ret
CarregaPaleta2 ENDP

;--------------------------------------------------------------------------------
; Proc PlotarFigura
; Desenha um Grafico na Tela
;
; Parametros
;   BX      Posicao do Grafico
;   CX      Largura do Grafico
;   DX      Altura do Grafico
;   DS:SI   Aponta para o Buffer do Grafico
;--------------------------------------------------------------------------------
PlotarFigura PROC NEAR ;   MACRO Altura, Largura, Posicao, OffsetFigura
    ; Empilha Registradores
    pusha
    push es di
 ;   mov cx, Largura
 ;   mov bx, Posicao
 ;   mov dx, Altura

    ; Zera DirectionFlag - faz DI incrementar apos MOVSB
    cld
  ;  mov si, OffsetFigura

    ; ES:DI Aponta para a Posicao na Memoria de Video
    mov ax, 0A000h
    mov es, ax
    mov di, bx

    ; Quantidade de Repeticoes Lineares (Largura da Figura)
    mov bx, cx

; Loop para a Escrita da Imagem na Memoria de Video
DGLoop:
    ; Copia Linha da Imagem do Buffer para a Memoria de Video
    mov cx, bx
    rep movsb

    ; Posiciona DI para a Proxima Linha
    sub di, bx
    add di, 320

    ; Decrementa Contador de Altura
    dec dx
    jnz DGLoop

    ; Desempilha Registradores
    pop di es
    popa
    ret
PlotarFigura ENDP

;******************************************
; Procedimento que verifica qual tecla eh pressionada durante o jogo
; para redesenhar a posicao de Sergio
;*******************************************
Jogar PROC NEAR
  pusha
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;Emanoel
    call NoSound
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;Emaanoel
  loops:

    cmp ds:[varLeft], 1
    jne right
    cmp posicao_tela, 57625                 ; (180*320)+30 limite esquerdo de Sergio
    jbe right ;se menor ou igual
    sub posicao_tela, 2
    call DesenhaSergio


  right:
    cmp ds:[varRight], 1
    jne tiro
    cmp posicao_tela, 57867                 ; limite direito de Sergio
    jge tiro                                ;se maior ou igual
    add posicao_tela, 2
    call DesenhaSergio

  tiro:
    cmp ds:[varEspaco], 1
    jne Pausar
    call Atirar
    call DesenhaSergio

    ;rotina que move o tiro

  Pausar:
    cmp ds:[varPause], 1
    jne sair
    call Pause

  sair:
  popa
  ret
Jogar ENDP

;************************************************
; Chama a tela que passa da fase 1 para a fase 2
;************************************************
fase1_2P PROC NEAR
  pusha
  push si

    mov ax, @DATA
    mov ds, ax

    mov dx, offset fases_pal                    ; carrega a figura na tela
    call CarregaPaleta                          ;
    mov dx, offset fases_dat                    ;
    call DesenhaTela                            ;

     mov si, offset fase1_2
     mov dh, 0EAh
     mov di, 33
     mov cl, 26
     mov podeSair, 1
     mov sair_hist, 0
     mov ds:[espera], 3
     call DesenharFrase
     cmp sair_hist, 1
     je pul2
     call esperarTecla
     pul2:
     mov sair_hist, 0
     mov podeSair, 0
  pop si
  popa
  ret
fase1_2P ENDP

;************************************************
; Proc que chama a tela da fase 2 para a 3
;************************************************

fase2_3P PROC NEAR
  pusha
  push si

    mov ax, @DATA
    mov ds, ax

    mov dx, offset fases_pal
    call CarregaPaleta
    mov dx, offset fases_dat
    call DesenhaTela

     mov si, offset fase2_3
     mov dh, 0EAh
     mov di, 33
     mov cl, 26
     mov podeSair, 1
     mov sair_hist, 0
     mov ds:[espera], 3
     call DesenharFrase
     cmp sair_hist, 1
     je pul1
     call esperarTecla
     pul1:
     mov sair_hist, 0
     mov podeSair, 0
  pop si
  popa
  ret
fase2_3P ENDP


;************************************************
; Proc que chama a tela de GameOver
;************************************************
GOver PROC NEAR
  pusha
  push si

    mov ax, @DATA
    mov ds, ax

    mov dx, offset over_pal
    call CarregaPaleta
    mov dx, offset over_dat
    call DesenhaTela

     mov si, offset gameover_hist
     mov dh, 0FFh
     mov di, 73
     mov cl, 18
     mov podeSair, 1
     mov sair_hist, 0
     mov ds:[espera], 3
     call DesenharFrase
     cmp sair_hist, 1
     je pul3
     call esperarTecla
     pul3:
     mov sair_hist, 0
     mov podeSair, 0
  pop si
  popa
  ret
GOver ENDP

;************************************************
; Procedimento principal que controla todos os movimentos de todos os componentes da tela
;
;************************************************

JogarMesmo   PROC NEAR
  push ax ds

  mov ax, @data
  mov ds, ax

  mov Pontos, 0
  mov Vidas, 3
  mov nivel, 1

  cmpJogarMesmo:
  cmp SergioMorreu, 0
  je inicBarreira
  call SoundMorreu
  jmp semSom

  inicBarreira:
  call InicializaBarreira

  semsom:
    cmp mudouFase, 1
    jne naoMudou
      call calculoScore
      cmp nivel, 2
      je ni2
      cmp nivel, 3
      je ni3
      ;cmp nivel, 4
      ;je GO
      jmp naoMudou

      ni2:
       call fase1_2P
       jmp naoMudou
      ni3:
       call fase2_3P
       jmp naoMudou
      ;GO:
      ; call GOver

    naoMudou:

    call Inicializar

    cmp nivel, 1
    je nivel1
    cmp nivel, 2
    je nivel2
    cmp nivel, 3
    je nivel3
    cmp nivel, 4
    je zerou
    jmp fimJogarMesmo

  nivel1:
    call NoSound
    mov limite_mano, 400
    mov lim1, 20
    mov lim2, 25
    mov linhaPulo, 4
    mov tamPulo, 1280
    mov espera, 5
    mov si, offset fase1
    mov dh, 0FFh
    mov di, 138
    mov cl, 97
    call DesenharFrase
    call esperarTecla
    call desenhaFundo
    call desenhaSergio
    mov espera, 1

  loopNivel1:
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
    call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar
    call AtirarInimigo

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
     je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo
    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
     je fimJogarMesmo


    call Delay
    call ContaTempo
    call DesenhaTempo
    call atirarInimigo      ;almenta velocidadade da fase um
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo
    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
     je fimJogarMesmo

    cmp nivel, 1
     je loopNivel1
     mov mudouFase, 1
     jmp cmpJogarMesmo

   nivel2:
   call NoSound
    mov limite_mano, 350
    mov lim1, 20
    mov lim2, 30
    mov linhaPulo, 2
    mov tamPulo, 640
    mov espera, 5
    mov si, offset fase2
    mov dh, 0FFh
    mov di, 138
    mov cl, 97
    call DesenharFrase
    call esperarTecla
    call desenhaFundo
    call desenhaSergio
    mov espera, 1

   loopNivel2:
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
    call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call AtirarInimigo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo


    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
 ;   call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    cmp nivel, 2
     je loopNivel2
     mov mudouFase, 1
     jmp cmpJogarMesmo

   nivel3:
   call NoSound
    mov limite_mano, 300
    mov lim1, 20
    mov lim2, 35
    mov linhaPulo, 2
    mov tamPulo, 640
    mov espera, 5
    mov si, offset fase3
    mov dh, 0FFh
    mov di, 138
    mov cl, 97
    call DesenharFrase
    call esperarTecla
    call desenhaFundo
    call desenhaSergio
    mov espera, 1

   loopNivel3:
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
    call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call AtirarInimigo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo
    call Delay
    call ContaTempo
    call DesenhaTempo
    call Jogar

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
  ;  call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo

    call Delay
    call ContaTempo
    call DesenhaTempo

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoverInimigo
    call AtirarInimigo
    ;call DesenhaEmanoel

    cmp GameOver, 1
     je labelGameOver
    cmp SergioMorreu, 1
     je cmpJogarMesmo

    call MoveTampa
    call MoveTampa
    call telaEsc
    cmp varEsc, 1
    je fimJogarMesmo

    cmp nivel, 3
     je loopNivel3
     mov mudouFase, 1
     jmp cmpJogarMesmo


 zerou:
    mov dx, offset zerou_pal
    call CarregaPaleta
    mov dx, offset zerou_dat
    call DesenhaTela

     mov si, offset zerou_hist
     mov dh, CrM
     mov di, 10
     mov cl, 40
     mov podeSair, 1
     mov sair_hist, 0
     mov ds:[espera], 3
     call DesenharFrase
     cmp sair_hist, 1
     je pul4
     call esperarTecla
     pul4:
     mov sair_hist, 0
     mov podeSair, 0

    jmp fimJogarMesmo

 labelGameOver:

 cmp SergioMorreu, 1
    jne semSomOver
    call SoundMorreu
    call GOver
    semSomOver:
 fimJogarMesmo:
 pop ds ax
 ret
JogarMesmo ENDP

;************************************************
; Procedimento que inicializa a barreira quando passa de fase, ou quando reinicia o jogo
;************************************************
InicializaBarreira PROC NEAR
  pusha
  push ds

  mov ax,@data
  mov ds, ax

  mov bx, 0

  loopInicBarreira:
   cmp bx, 483
   je acabouLIB
   mov al, biroDefault[bx]
   mov biro1[bx], al
   mov biro2[bx], al
   inc bx
  jmp loopInicBarreira

  acabouLIB:
  pop ds
  popa
  ret
InicializaBarreira ENDP

;************************************************
;Procedimento que carrega a tela que pergunta se quer realmente sair do jogo
;Se a tecla s for pressionada, volta pro menu Inicial
;Se a tecla n for pressionada, volta para o jogo (do mesmo ponto em que parou)
;************************************************

telaEsc PROC NEAR
  push ax ds dx
  mov ax, @data
  mov ds, ax

  cmp varEsc, 0
  je fimTE

  call DesenhaFundo
  call DesenhaSergio
  mov si, offset msgSair
  mov dh, 0FFh
  mov di, 70
  mov cl, 97
  call DesenharFrase
  call NoSound
  mov varS, 0
  mov varN, 0

  loopTelaEsc:
    cmp varS, 1
    je disse_sim
    cmp varN, 1
    je disse_nao
    jmp loopTelaEsc

  disse_sim:
  jmp fimTE

  disse_nao:
    mov varEsc, 0
    call DesenhaFundo
    call DesenhaSergio
    call DesenhaInimigos
    call DesenhaTampa
    call DesenhaZ
  fimTE:

  pop dx ds ax
  ret
telaEsc ENDP

;************************************************
;Procedimento que desenha a tampa na tela
;************************************************

DesenhaTampa PROC NEAR
  pusha
  push si

  mov ax, @data
  mov ds, ax
  mov bx, posicaoTampa
  mov cx, 3
  mov dx, 6
  mov si, offset Tampa
  call PlotarFigura

  pop si
  popa
  ret
DesenhaTampa ENDP

;************************************************
; Procedimento que desenha e move "Emanoel" na tela
; Alem disso, apaga "Emanoel" quando ele chega ao limite da tela
;************************************************
DesenhaEmanoel PROC NEAR
  pusha
  push ds si

  mov ax, @data
  mov ds, ax

  cmp linha, 33         ; compara se as carinhas jah tao na posicao valida
  jb fimDE

  cmp mano_na_tela, 1
  je moveEmanoel


  mov dx, limite_mano
  call GeraNumeroAleatorio

  cmp bx, 1
  jne fimDE

  mov mano_na_tela, 1
  cmp mano_indo_volta, 0
  je manoDireita

  mov mano_indo_volta, 0
  mov si, offset EmanoelEsq
  mov posicao_mano, 7626
  mov bx, posicao_mano
  mov cx, 34
  mov dx, 11
  call PlotarFigura
  jmp moveEmanoel

  manoDireita:
    mov mano_indo_volta, 1
    mov si, offset EmanoelDir
    mov posicao_mano, 7395
    mov bx, posicao_mano
    mov cx, 34
    mov dx, 11
    call PlotarFigura
    jmp moveEmanoel

  moveEmanoel:
    cmp mano_indo_volta, 0           ; Emanoel tah indo pra esquerda(pois alterou o valor dele lah em cima!)
    je manoEsq

  manoDir:
    add posicao_mano, 2
    cmp posicao_mano, 7626
    jge apagarMano
    mov si, offset EmanoelDir
    mov bx, posicao_mano
    mov cx, 34
    mov dx, 11
    call PlotarFigura
    call ChoqueTampaEmanoel
    jmp fimDE

  manoEsq:
    sub posicao_mano, 2
    cmp posicao_mano, 7395
    jle apagarMano
    mov si, offset EmanoelEsq
    mov bx, posicao_mano
    mov cx, 34
    mov dx, 11
    call PlotarFigura
    call ChoqueTampaEmanoel
    jmp fimDE

  apagarMano:
    mov mano_na_tela, 0
    mov si, offset LimpaEmanoel
    mov bx, posicao_mano
    mov cx, 34
    mov dx, 11
    call PlotarFigura

  fimDE:
  pop si ds
  popa
  ret
DesenhaEmanoel ENDP

;************************************************
;Procedimento que desenha os "Z" na tela
;************************************************

DesenhaZ  PROC NEAR
  pusha
  push si
  mov ax, @data
  mov dx, ax

  mov ax, 0

  loopDI:
    cmp ax, 11
    je fimDI
    mov bx, ax
    inc ax
    shl bx, 1
    mov dx, tirosInimigos[bx]
    cmp dx, 0
    je loopDI
    pusha
    mov bx, dx
    mov cx, 3
    mov dx, 6
    mov si, offset tiroInimigo
    call Plotarfigura
    popa
    jmp loopDI

  fimDI:
  pop si
  popa
  ret
DesenhaZ ENDP

;************************************************
; Procedimento que Pausa o jogo
;************************************************
Pause PROC NEAR
  pusha
  mov ax, @data
  mov ds, ax
  call NoSound
  loopPause:
    cmp varPause, 0                   ; se nao pressionou vai pro fim
    je fimPause
    cmp varEsc, 1
    je fimPause
    mov si, offset strpause
    mov dh, 0FFh
    mov di, 138
    mov cl, 97
    mov espera, 5
    call DesenharFrase
    jmp loopPause
  fimPause:
  mov espera, 1
  call DesenhaFundo
  call DesenhaSergio

  popa
  ret
Pause ENDP

;********************************************************
; 
; Procedimento que desenha o fundo da tela (a sala de aula)
;Desenha o Score, as Vidas ,o Tempo e as Barreiras
;
;********************************************************
DesenhaFundo PROC NEAR

  push ax ds dx
  mov ax, @data
  mov ds, ax
  mov dx, offset borda_pal
  call CarregaPaleta
  mov dx, offset borda_dat
  call DesenhaTela
  call DesenhaScore
  call DesenhaVidas
  call desenhaTempo
  call DesenhaBarreira
  pop dx ds ax
  ret
DesenhaFundo ENDP


;******************************************************
;Procedimento que desenha os fundos sem as Barreiras
;******************************************************

DesenhaFundo2 PROC NEAR

  push ax ds dx
  mov ax, @data
  mov ds, ax
  mov dx, offset borda_pal
  call CarregaPaleta
  mov dx, offset borda_dat
  call DesenhaTela
  call DesenhaScore
  call DesenhaVidas
  call desenhaTempo
  pop dx ds ax
  ret
DesenhaFundo2 ENDP

;******************************************************
; Procedimento que chama a tela do Score
; e calcula o Score total
; ScoreTotal = Score + (vidasRestantes*600) - (tempoDecorrido*3)
;******************************************************
calculoScore PROC NEAR
  call NoSound
  pusha
  push si

  call desenhaFundo2
  mov podeSair, 0
  mov ds:[espera], 0

  mov al, vidas
  mov tempVida, al
  mov ax, pontos
  mov tempScore, ax
  mov ax, timeCounter
  mov tempTempo, ax

  mov si, offset pontosVIdas
  mov dh, 0FFh
  mov di, 90
  mov cl, 70
  call DesenharFrase

  mov ax, 600
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 70
  call DesenharNumero

  mov si, offset penalidade
  mov dh, 0FFh
  mov di, 90
  mov cl, 100
  call DesenharFrase

  mov ax, 0
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 100
  call DesenharNumero

  mov si, offset scoreTotal
  mov dh, 0FFh
  mov di, 90
  mov cl, 130
  call DesenharFrase

  mov ax, 0
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 130
  call DesenharNumero


  mov espera, 30
  call delay

  loopVida:
    call NoSound
    mov espera, 5
    call delay
    mov bx, 1000
    call PlaySound
    call delay
    cmp vidas, 0
    je saiVidas
    dec vidas
    call desenhaVidas
    mov espera, 0
    mov si, offset cinza
    mov bx, 22594
    mov cx, 35
    mov dx, 9
    call plotarFigura
    mov ax, totalvidas
    add ax, 600
    mov totalvidas, ax
    push ax
    call converterDecimal
    mov dh, 0ffh
    mov di, 194
    mov ah, 70
    call DesenharNumero
    pop ax
    jmp loopVida

saiVidas:
call NoSound
  mov al, tempVida
  mov vidas, al
  mov espera, 30
  call delay

loopPenalidade:
   call NoSound
  mov espera, 1
  call delay
  mov bx, 1000
  call PlaySound
  call Delay
  cmp timeCounter, 0
  je saiPenalidade
  dec timeCounter
  call desenhatempo
  mov espera, 0
  mov si, offset cinza
  mov bx, 32194
  mov cx, 35
  mov dx, 9
  call plotarFigura
  mov ax, totalTempo
  add ax, 3
  mov totaltempo, ax
  push ax
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 100
  call DesenharNumero
  pop ax
  jmp loopPenalidade

saiPenalidade:
  call NoSound
  mov ax, tempTempo
  mov timeCounter, ax
  mov espera, 30
  call delay

loopScore:
  call NoSound
  mov espera, 0
  call delay
  mov bx, 1000
  call PlaySound
  call delay
  cmp pontos, 0
  je saiScore
  dec pontos
  call desenhaScore
  mov espera, 0
  mov si, offset cinza
  mov bx, 41794
  mov cx, 35
  mov dx, 9
  call plotarFigura
  mov ax, totalscore
  add ax, 1
  mov totalscore, ax
  push ax
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 130
  call DesenharNumero
  pop ax
  jmp loopScore

saiScore:
  call NoSound
  mov ax, totalScore
  mov pontos, ax

mov ax, totalscore
add ax, totalvidas
mov totalvidas, 0

mov totalScore, ax
mov pontos, ax
  mov espera, 20
  call delay

  mov espera, 0
  mov si, offset cinza
  mov bx, 22594
  mov cx, 35
  mov dx, 9
  call plotarFigura

  mov ax, 0
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 70
  call DesenharNumero


  mov espera, 0
  mov si, offset cinza
  mov bx, 41794
  mov cx, 35
  mov dx, 9
  call plotarFigura

  mov ax, pontos
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 130
  call DesenharNumero

  mov ax, totalscore
  sub ax, totaltempo

  mov totalScore, ax
  mov pontos, ax
  mov totaltempo, 0

  mov espera, 20
  call delay

  mov espera, 0
  mov si, offset cinza
  mov bx, 32194
  mov cx, 35
  mov dx, 9
  call plotarFigura

  mov ax, 0
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 100
  call DesenharNumero


  mov espera, 0
  mov si, offset cinza
  mov bx, 41794
  mov cx, 35
  mov dx, 9
  call plotarFigura

  mov ax, pontos
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 130
  call DesenharNumero

  mov espera, 30
  call delay

  mov espera, 0
  mov si, offset cinza
  mov bx, 41794
  mov cx, 35
  mov dx, 9
  call plotarFigura

  mov ax, 0
  call converterDecimal
  mov dh, 0ffh
  mov di, 194
  mov ah, 130
  call DesenharNumero

  call desenhaScore
  mov bx, 1500
  call PlaySound
  mov espera, 2
  call delay
  call NoSound


  mov si, offset teclas
  mov dh, 0FFh
  mov di, 40
  mov cl, 160
  call DesenharFrase

  call esperarTecla

  mov espera, 1
  pop si
  popa
  ret


calculoScore ENDP

;**********************************************************
;
; procedimento para desenhar Sergio, na parte de baixo
; da tela
;
;********************************************************
DesenhaSergio PROC NEAR

  push ax ds si bx cx dx
  mov ax, @data
  mov ds, ax

  cmp tampa_na_tela, 0
  jne trocaSergio

  mov si, offset ProfessorTampa
  jmp plt

  trocaSergio:
  mov si, offset ProfessorSemTampa
  jmp plt


  plt:
  mov bx, posicao_tela
  mov cx, 31                 ;largura
  mov dx, 18                 ;altura

  call PlotarFigura

  pop dx cx bx si ds ax
  ret
DesenhaSergio ENDP


;******************************************************
;Procedimento que desenha a tampa na tela relativa a Sergio
;******************************************************

Atirar PROC   NEAR
  pusha
  mov ax,@data
  mov ds, ax
  mov bx, posicao_tela
  add bx, 25                     ; posicao da tampa com relacao a Sergio
  mov dx, bx
  sub dx, 49280                 ; (154*320)
  mov coluna, dx
  mov posicaoTampa, bx
  mov cx, 3
  mov dx, 6
  mov si, offset Tampa
  call PlotarFigura
  mov tampa_na_tela,1
  mov varEspaco,0
  mov bx, 800
  call PlaySound
  popa
  ret
Atirar ENDP

;******************************************************
; Procedimento que desenha os "Z" e os move na tela
;******************************************************

AtirarInimigo PROC NEAR
  pusha
  push si ds
  mov ax, @data
  mov ds, ax

  cmp balas_na_tela, 12
  je fimAtirar
  mov dx, 100
  call GeraNumeroAleatorio

  cmp bx, lim1
  jl fimAtirar
  cmp bx, lim2
  jg fimAtirar

  mov dx, 19
  call GeraNumeroAleatorio
  shl bx, 1
  cmp array_Inimigo[bx], -1
  je fimAtirar
  mov dx, array_Inimigo[bx]
  add dx, posicaoInimigo
  add dx, 5766               ; pra sair do meio do inimigo
  mov cx, 0

  loopV:
    cmp cx, 11
    je fimAtirar
    mov bx, cx
    shl bx, 1
    inc cx
    cmp tirosInimigos[bx], 0
    jne loopV
    mov tirosInimigos[bx], dx
    inc balas_na_tela
    mov bx, dx
    mov cx, 3
    mov dx, 6
    mov si, offset tiroInimigo
    call PlotarFigura
    ;jmp fimMover

  fimAtirar:
  mov cx, 0
  loopfimAtirar:
    cmp cx, 11                         ; quantidade de (balas-1) que sera usada para o calculo do indice
    je fimMover
    mov bx, cx
    inc cx
    shl bx, 1
    mov dx, tirosInimigos[bx]
    cmp dx, 0
    je loopfimAtirar                   ;fimMover
    add dx, 320
    cmp dx, 59520                      ; limite inferior do "Z" na tela
    ja apagarTiro
    push bx dx cx si
    mov bx, dx
    mov cx, 3
    mov dx, 6
    mov si, offset tiroInimigo
    call PLotarFigura
    call ChoqueZProf
    call ChoqueTampaZ
    call ChoqueZBarreira
    call ChoqueZBarreira2
    pop si cx dx bx
    cmp SergioMorreu, 1
    je fimMover
    mov tirosInimigos[bx], dx
    jmp loopfimAtirar

  apagarTiro:
    push bx dx cx si
    mov bx, dx
    mov cx, 3
    mov dx, 6
    mov si, offset limpaTiroInimigo
    call PLotarFigura
    pop si cx dx bx
    mov tirosInimigos[bx],0
    dec balas_na_tela
    jmp loopfimAtirar
  fimMover:
  pop ds si
  popa
  ret
AtirarInimigo ENDP

;******************************************************
; Procedimento que move a tampa na tela
;******************************************************

MoveTampa  PROC  NEAR
  pusha
  push si
  mov ax, @data
  mov ds, ax

  cmp tampa_na_tela, 0
  je fimMesmo
  mov bx, posicaoTampa
  sub bx, 320
  mov posicaoTampa, bx
  mov cx, 3
  mov dx, 6
  cmp bx, coluna
  je limpa
  mov si, offset Tampa
  jmp fimMove
  limpa:
  mov tampa_na_tela, 0
  mov varEspaco, 0
  mov posicaoTampa, 0
  mov si, offset limpaTampa
  call DesenhaSergio
  jmp fimMove
  fimMove:
  call PlotarFigura
  call ChoqueTampa
  call ChoqueTampaZ
  call ChoqueTampaBarreira
  call ChoqueTampaBarreira2
  call ChoqueTampaEmanoel
  fimMesmo:
  pop si
  popa
  ret
MoveTampa ENDP

;******************************************************
; Procedimento que desenha os inimigos na tela
; e alterna entre os inimigos acordados e os dormindo
;******************************************************

DesenhaInimigos PROC NEAR
  pusha
  push ds
  push si
  mov ax, @data                       ; aponta pro segmento de dados
  mov ds, ax
  mov ax, 0                           ; inicializa o contador com o numero total de inimigos
  mov cx, 13                          ; largura da figura do inimigo
  mov dx, 18                          ; altura da figura


  loopDesenha:
    cmp ax, 20
    je saiDae
    mov bx, ax
    shl bx, 1                           ; multiplica bx por 2

    cmp bx, 2                           ; posicao de Aninha no array
    je desenhaAninha
    cmp bx, 4
    je desenhaCarina                    ; posicao de Carina no array
    jmp meninos

    desenhaAninha:
    cmp boolN, 0
    jne aninhaAcordada
    mov si, offset inimigaA1
    jmp plotar

    aninhaAcordada:
    mov si, offset inimigaA2
    jmp plotar

    desenhaCarina:
    cmp boolN, 0
    jne carinaAcordada
    mov si, offset inimigaC1
    jmp plotar

    carinaAcordada:
    mov si, offset inimigaC2
    jmp plotar

    meninos:
    cmp boolN, 0
    jne tahacordado
    mov si, offset inimigo
    jmp plotar

    tahacordado:
    mov si, offset inimigo2

    plotar:
    mov bx, array_inimigo[bx]           ; move pra bx a posicao do inimigo que sera escrito
    inc ax
    cmp bx, -1
    je loopDesenha
    add bx, posicaoInimigo
    call PlotarFigura
    jmp loopDesenha

  saiDae:
    pop si
    pop ds
    popa
    ret
DesenhaInimigos ENDP

;******************************************************
;Procedimento que move os inimigos na tela
;******************************************************

MoverInimigo PROC NEAR

  pusha
  mov ax, @data
  mov ds, ax
  cmp direcaoInimigos, 1                  ; compara se eh direita ou esquerda
  je DireitaMI

  EsquerdaMI:
  mov al, linha
  mov ah, 0h
  mov dx, 320
  mul dx
  add ax, 35                              ; soma com o tamanho da borda esquerda
  mov bx, posicaoInimigo
  add bx, var_esquerdo                    ; soma a posicao do inimigo com a var_esquerdo que eh zero inicialmente
  cmp bx, ax                              ; ela serve pra alterar o ponto de referencia da posicaoInimigo
  jle inverte_pula
  sub posicaoInimigo, 2                   ; subtrai 2 da posicaoInimigo que eh o quanto ele anda na tela
  jmp fimMoverInimigo

  DireitaMI:
  mov al, linha
  mov ah, 0h
  mov dx, 320
  mul dx
  add ax, 300
  mov bx, posicaoInimigo
  add bx, var_direito
  cmp bx, ax
  jge inverte_pula
  add posicaoInimigo, 2                   ; soma 2 da posicaoInimigo que eh o quanto ele anda na tela
  jmp fimMoverInimigo

  inverte_pula:
  cmp direcaoInimigos, 1
  je bota_zero
  mov direcaoInimigos, 1
  jmp pular
  bota_zero:
  mov direcaoInimigos, 0

  pular:
  push ax cx
  mov al, linhaPulo
  mov cx, tamPulo
  add linha, al;2
  add posicaoInimigo, cx;640
  pop cx ax

  fimMoverInimigo:
  cmp cont, 0
  je mudarcont
  dec cont
  jmp fimCont

  mudarcont:
  push ax
  mov cont, 30
  mov al, 1
  sub al, boolN
  mov boolN, al
  pop ax

  fimCont:
  call choqueIniBar
  call choqueIniProf
  call DesenhaInimigos
  call ChoqueTampa
  popa
  ret
MoverInimigo ENDP

;******************************************************
; Procedimento que verifica quando uma coluna de inimigos foi
; eliminada para entao alterar o limite em que eles se movem na tela
;******************************************************

ColunaInimigo   PROC NEAR
  pusha

  mov ax, @data
  mov ds, ax
  cmp direcaoInimigos, 1
  je cmpDireita

  cmpEsquerda:
  cmp coluna1, 0       ; cmp coluna1 com zero
  jne fimColIni        ; se nao for ele sai do proc

  cmp coluna2, 0       ; se col1 for zero, cmp a col2
  jne colEsq2

  cmp coluna3, 0
  jne colEsq3

  cmp coluna4, 0
  jne colEsq4

  cmp coluna5, 0
  jne colEsq5
  jmp fimColIni

  colEsq2:             ; a coluna 1 eh zero, mas a dois nao
  mov var_esquerdo, 30
  jmp fimColIni

  colEsq3:             ; a coluna 1 eh zero, a dois tb, mas a 3nao
  mov var_esquerdo, 60
  jmp fimColIni

  colEsq4:             ; a coluna 1, a dois, a 3 sao zero, mas a 4 nao
  mov var_esquerdo, 90
  jmp fimColIni

  colEsq5:
  mov var_esquerdo, 120
  jmp fimColIni

  cmpDireita:

  cmp coluna5, 0       ; cmp coluna1 com zero
  jne fimColIni        ; se nao for ele sai do proc

  cmp coluna4, 0       ; se col1 for zero, cmp a col2
  jne colDir4

  cmp coluna3, 0
  jne colDir3

  cmp coluna2, 0
  jne colDir2

  cmp coluna1, 0
  jne colDir1
  jmp fimColIni

  colDir4:             ; a coluna 1 eh zero, mas a dois nao
  mov var_direito, 103
  jmp fimColIni

  colDir3:             ; a coluna 1 eh zero, a dois tb, mas a 3nao
  mov var_direito, 73
  jmp fimColIni

  colDir2:             ; a coluna 1, a dois, a 3 sao zero, mas a 4 nao
  mov var_direito, 43
  jmp fimColIni

  colDir1:
  mov var_direito, 13
  jmp fimColIni

  fimColIni:
  popa
  ret
ColunaInimigo ENDP

;******************************************************
;Procedimento que verifica se houve choque entre a tampa e 
; algum inimigo
;******************************************************

ChoqueTampa  PROC NEAR
  pusha
  push ds si
  mov ax, @data
  mov ds, ax
  mov bx, 0
  mov dx, posicaoTampa
 ; sub dx, 320  ; ALTERAÇÕA
  mov cx, 0

  lacotampaz:             ;
  cmp cx, 4               ;
  je saiChoque            ;  ALTERAÇÃO
  inc cx                  ;
  mov ax, 320             ;  Estas alterações servem para que o choque considere toda a Tampa, ou seja a matriz 3X4
  mul cx                  ;
  mov dx, posicaoTampa    ;
  add dx, ax              ;
  mov bx, 0               ;


  lacoChoque:
    cmp bx, 40
    je lacoTampaZ        ;ALTERAÇÃO ESTAVA: saiChoque
    mov ax, array_inimigo[bx]
    add bx, 2
    cmp ax, -1
    je lacoChoque
    add ax, posicaoInimigo
    add ax, 3202             ; primeira linha [(11*320)+3]
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8                ; soma com a largura da carinha
    sub dx, 2
    cmp dx, ax
    ja lacoChoque1
    jmp houveChoque

    lacoChoque1:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque2
    jmp houveChoque

    lacoChoque2:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque3
    jmp houveChoque

    lacoChoque3:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque4
    jmp houveChoque

    lacoChoque4:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque5
    jmp houveChoque

    lacoChoque5:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque6
    jmp houveChoque

    lacoChoque6:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque7
    jmp houveChoque

    lacoChoque7:
    add ax, 312
    add dx, 2
    cmp dx, ax
    jb lacoChoque
    add ax, 8
    sub dx, 2
    cmp dx, ax
    ja lacoChoque


    houveChoque:
    push bx
    mov bx, 400
    call PlaySound
    pop bx
    pusha
    push si
    dec inimigos_vivos
    sub bx, 2

    call ContarScore
    cmp bx, 0        ;
    je subda1        ;
    cmp bx, 2        ;
    je subda2        ;
    cmp bx, 4        ; verifica se o bonequinho q morreu era da linha1
    je subda3        ;
    cmp bx, 6        ;
    je subda4        ;
    cmp bx, 8        ;
    je subda5        ;

    cmp bx, 10       ;
    je subda1        ;
    cmp bx, 12       ;
    je subda2        ;
    cmp bx, 14       ; verifica se o bonequinho q morreu era da linha2
    je subda3        ;
    cmp bx, 16       ;
    je subda4        ;
    cmp bx, 18       ;
    je subda5        ;

    cmp bx, 20       ;
    je subda1        ;
    cmp bx, 22       ;
    je subda2        ;
    cmp bx, 24       ; verifica se o bonequinho q morreu era da linha3
    je subda3        ;
    cmp bx, 26       ;
    je subda4        ;
    cmp bx, 28       ;
    je subda5        ;

    cmp bx, 30       ;
    je subda1        ;
    cmp bx, 32       ;
    je subda2        ;
    cmp bx, 34       ; verifica se o bonequinho q morreu era da linha4
    je subda3        ;
    cmp bx, 36       ;
    je subda4        ;
    cmp bx, 38       ;
    je subda5        ;

  voltaSubtrai:
    mov ax, array_Inimigo[bx]
    add ax, posicaoInimigo
    mov array_Inimigo[bx], -1
    mov bx, ax
    mov cx, 13
    mov dx, 18
    mov si, offset limpaInimigo
    call PlotarFigura
    mov bx, posicaoTampa
    mov cx, 3
    mov dx, 6
    mov si, offset limpaTampa
    mov tampa_na_tela, 0
    mov varEspaco, 0
    mov posicaoTampa, 0
    call PlotarFigura
    call DesenhaSergio
    call VerificaNivel
    pop si
    popa
    jmp saiChoque

  subda1:
   dec coluna1
   jmp voltaSubtrai
  subda2:
   dec coluna2
   jmp voltaSubtrai
  subda3:
   dec coluna3
   jmp voltaSubtrai
  subda4:
   dec coluna4
   jmp voltaSubtrai
  subda5:
   dec coluna5
   jmp voltaSubtrai

  saiChoque:
    call ColunaInimigo
    pop si ds
    popa
    ret
ChoqueTampa  ENDP

;******************************************************
; Procedimento que trata o choque do "Z" com o professor
;******************************************************
ChoqueZProf PROC NEAR
  pusha
  push ds si

  mov ax, @data
  mov ds, ax
  mov bx, 0
  mov dx, posicao_tela          ; mpve pra dx a posicao do professor na tela

  loopCZP:
   cmp bx, 24
   je fimCZP
   mov dx, posicao_tela
   mov ax, tirosInimigos[bx]    ; move pra ax a posicao do "Z"
   add bx, 2                    ; incrementa bx
   cmp ax, 0
   je loopCZP

   add ax, 1602                 ; 1602 = (5*320) + 2, parte mais abaixo e mais a direita do "Z"
   add dx, 329                  ; valor da primeira linha considerada no professor
   cmp ax, dx
   jb loopCZP
   sub ax, 2
   add dx, 11
   cmp ax, dx
   ja loopC1
   jmp houveChoqueZ

  loopC1:
   mov dx, posicao_tela
   add dx, 649
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 11
   cmp ax, dx
   ja loopC2
   jmp houveChoqueZ

  loopC2:
   mov dx, posicao_tela
   add dx, 969
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 11
   cmp ax, dx
   ja loopC3
   jmp houveChoqueZ

  loopC3:
   mov dx, posicao_tela
   add dx, 1289
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 19
   cmp ax, dx
   ja loopC4
   jmp houveChoqueZ

  loopC4:
   mov dx, posicao_tela
   add dx, 1609
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 19
   cmp ax, dx
   ja loopC5
   jmp houveChoqueZ

  loopC5:
   mov dx, posicao_tela
   add dx, 1929
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 19
   cmp ax, dx
   ja loopC6
   jmp houveChoqueZ

  loopC6:
   mov dx, posicao_tela
   add dx, 2249
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 19
   cmp ax, dx
   ja loopC7
   jmp houveChoqueZ

 loopC7:
   mov dx, posicao_tela
   add dx, 2569
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 19
   cmp ax, dx
   ja loopC8
   jmp houveChoqueZ

 loopC8:
   mov dx, posicao_tela
   add dx, 2882
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 26
   cmp ax, dx
   ja loopC9
   jmp houveChoqueZ

 loopC9:
   mov dx, posicao_tela
   add dx, 3202
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 26
   cmp ax, dx
   ja loopC10
   jmp houveChoqueZ

 loopC10:
   mov dx, posicao_tela
   add dx, 3522
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 26
   cmp ax, dx
   ja loopC11
   jmp houveChoqueZ

 loopC11:
   mov dx, posicao_tela
   add dx, 3842
   add ax, 2                    ; move pra ax a parte direita do "Z"
   cmp ax, dx
   jb loopCZP
   sub ax, 2                    ; move pra ax a parte esquerda do "Z"
   add dx, 26
   cmp ax, dx
   ja loopCZP
   jmp houveChoqueZ

 houveChoqueZ:

   sub Vidas, 1
   mov SergioMorreu, 1
   cmp Vidas, -1
   jne fimCZP
   mov GameOver, 1
   call DesenhaVidas
  fimCZP:
  pop si ds
  popa
  ret
ChoqueZProf ENDP

;******************************************************
; Procedimento que seta a variavel Pontos, de acordo
; o valor do inimigo que morreu
;******************************************************

ContarScore PROC NEAR
  pusha
  push si di

    mov ax, @data
    mov ds, ax

    cmp bx, 2
    je adic15
    cmp bx, 4
    je adic15

    add Pontos, 10
    jmp fimCS

    adic15:
    add Pontos, 15

    fimCS:
    call DesenhaScore
  pop di si
  popa
  ret
ContarScore ENDP

;******************************************************
; Procedimento que verifica o choque da tampa com o "Z"
;******************************************************

ChoqueTampaZ PROC NEAR
  pusha
  push ds si

  mov ax, @data
  mov ds, ax

  mov bx, 0                     ; move pra bx a posicao inicial do array dos "Z"
  mov dx, posicaoTampa          ; dx recebe a posicao da tampa na tela

  loopCTZ:

    cmp bx, 24                  ; eh a posicao maxima do "z"
    je fimCTZ
    mov ax, tirosInimigos[bx]
    add bx, 2
    cmp ax, 0                   ; verifica se o "Z" esta na tela no momento
    je loopCTZ

    add ax, 640                 ;(320*5) pra pular as 5 primeiras linhas da figura do "Z"
    add dx, 3                   ; pega a parte mais a direita da tampa
    cmp dx, ax
    jb loopCTZ

    sub dx, 3                   ; pega a parte mais a esquerda da tampa
    add ax, 2                   ; pega a parte mais a direita do "Z"
    cmp dx, ax
    ja loopCTZ2
    jmp houveChoqueTZ

  loopCTZ2:

    add ax, 319
    add dx, 3
    cmp dx, ax
    jb loopCTZ

    sub dx, 3
    add ax, 2
    cmp dx, ax
    ja loopCTZ3
    jmp houveChoqueTZ

  loopCTZ3:

    add ax, 319
    add dx, 3
    cmp dx, ax
    jb loopCTZ

    sub dx, 3
    add ax, 2
    cmp dx, ax
    ja loopCTZ4
    jmp houveChoqueTZ

  loopCTZ4:

    add ax, 319
    add dx, 3
    cmp dx, ax
    jb loopCTZ

    sub dx, 3
    add ax, 2
    cmp dx, ax
    ja loopCTZ
    jmp houveChoqueTZ

  houveChoqueTZ:
    sub bx, 2
    mov ax, tirosInimigos[bx]
    mov tirosInimigos[bx], 0
    dec balas_na_tela
    mov tampa_na_tela, 0
    mov varEspaco, 0
    mov si, offset limpaTiroInimigo
    mov bx, ax
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    mov si, offset limpaTampa
    mov bx, posicaoTampa
    mov posicaoTampa, 0
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    call DesenhaSergio

  fimCTZ:
  pop si ds
  popa
  ret
ChoqueTampaZ ENDP

;******************************************************
; Procedimento que desenha as barreiras na tela
;******************************************************

DesenhaBarreira PROC NEAR
  pusha
  push ds si

  mov ax, @data
  mov ds, ax

  mov bx, 50320
  mov cx, 44
  mov dx, 11
  mov si, offset biro1
  call PlotarFigura

  mov bx, 50433
  mov cx, 44
  mov dx, 11
  mov si, offset biro2
  call PlotarFigura

  pop si ds
  popa
  ret
DesenhaBarreira ENDP

;******************************************************
; Procedimento que verifica o choque da tampa com a barreira 1
;******************************************************

ChoqueTampaBarreira PROC NEAR
  pusha
  push ds si
  push di

  mov ax, @data
  mov ds, ax

   mov ax, posicaoTampa    ; move pra ax a posicao da "Tampa"
   cmp tampa_na_tela, 0
   je fimCTB

   mov di, 440
   mov dx, 53520
   cmp ax, dx
   jb CTB2
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB2:
   mov di, 396
   mov dx, 53200
   cmp ax, dx
   jb CTB3
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB3:
   mov di, 352
   mov dx, 52880
   cmp ax, dx
   jb CTB4
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB4:
   mov di, 308
   mov dx, 52560
   cmp ax, dx
   jb CTB5
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB5:
   mov di, 264
   mov dx, 52240
   cmp ax, dx
   jb CTB6
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB6:
   mov di, 220
   mov dx, 51920
   cmp ax, dx
   jb CTB7
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB7:
   mov di, 176
   mov dx, 51600
   cmp ax, dx
   jb CTB8
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB8:
   mov di, 132
   mov dx, 51280
   cmp ax, dx
   jb CTB9
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB9:
   mov di, 88
   mov dx, 50960
   cmp ax, dx
   jb CTB10
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB10:
   mov di, 44
   mov dx, 50640
   cmp ax, dx
   jb CTB11
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB

   CTB11:
   mov di, 0
   mov dx, 50320
   cmp ax, dx
   jb fimCTB
   add dx, 41
   cmp ax, dx
   ja fimCTB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueTB
   jmp fimCTB

houveChoqueTB:
    push ax dx

    mov si, offset limpaTampa
    mov tampa_na_tela, 0
    mov varEspaco, 0
    mov bx, posicaoTampa
    mov posicaoTampa, 0
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    call desenhaSergio
    pop dx ax
   ;mov dx, 50320
   sub ax, dx
   add ax, di
   mov bx, ax
   mov biro1[bx], 07h
   mov biro1[bx+1],07h
   mov biro1[bx+2], 07h
   mov biro1[bx-43], 07h
   call DesenhaBarreira


  fimCTB:
  pop di
  pop si ds
  popa
  ret

ChoqueTampaBarreira ENDP

;******************************************************
; Procedimento que verifica o choque da tampa com a barreira 2
;******************************************************      

ChoqueTampaBarreira2 PROC NEAR
  pusha
  push ds si
  push di

  mov ax, @data
  mov ds, ax

   mov ax, posicaoTampa    ; move pra ax a posicao da "Tampa"
   cmp tampa_na_tela, 0
   je fimCTB2

   mov di, 440
   mov dx, 53633
   cmp ax, dx
   jb CTB22
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB22:
   mov di, 396
   mov dx, 53313
   cmp ax, dx
   jb CTB32
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB32:
   mov di, 352
   mov dx, 52993
   cmp ax, dx
   jb CTB42
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB42:
   mov di, 308
   mov dx, 52673
   cmp ax, dx
   jb CTB52
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB52:
   mov di, 264
   mov dx, 52353
   cmp ax, dx
   jb CTB62
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB62:
   mov di, 220
   mov dx, 52033
   cmp ax, dx
   jb CTB72
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB72:
   mov di, 176
   mov dx, 51713
   cmp ax, dx
   jb CTB82
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB82:
   mov di, 132
   mov dx, 51393
   cmp ax, dx
   jb CTB92
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB92:
   mov di, 88
   mov dx, 51073
   cmp ax, dx
   jb CTB102
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB102:
   mov di, 44
   mov dx, 50753
   cmp ax, dx
   jb CTB112
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2

   CTB112:
   mov di, 0
   mov dx, 50433
   cmp ax, dx
   jb fimCTB2
   add dx, 41
   cmp ax, dx
   ja fimCTB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueTB2
   jmp fimCTB2

houveChoqueTB2:
    push ax dx

    mov si, offset limpaTampa
    mov tampa_na_tela, 0
    mov varEspaco, 0
    mov bx, posicaoTampa
    mov posicaoTampa, 0
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    call desenhaSergio
    pop dx ax
   ;mov dx, 50320
   sub ax, dx
   add ax, di
   mov bx, ax
   mov biro2[bx], 07h
   mov biro2[bx+1],07h
   mov biro2[bx+2], 07h
   mov biro2[bx-43], 07h
   call DesenhaBarreira


  fimCTB2:
  pop di
  pop si ds
  popa
  ret

ChoqueTampaBarreira2 ENDP


;******************************************************
; Procedimento que verifica o choque do "Z" com a barreira1
;******************************************************

ChoqueZBarreira  PROC NEAR
  pusha
  push ds si
  push di

  mov ax, @data
  mov ds, ax
  mov bx, 0

  loopCZB:
   cmp bx, 24
   je fimCZB
   mov ax, tirosInimigos[bx]    ; move pra ax a posicao do "Z"
   add bx, 2                    ; incrementa bx
   cmp ax, 0
   je loopCZB

   add ax, 1600                 ; parte mais abaixo e mais a esquerda do "Z"
   mov di, 0
   mov dx, 50320
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB2
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB2:
   mov di, 44
   mov dx, 50640
  ; mov ax, tirosInimigos[bx-2]    ; move pra ax a posicao do "Z"
  ; add ax, 1600
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB3
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

  loopCB3:
  mov di, 88
   mov dx, 50960
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB4
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

  loopCB4:
  mov di, 132
   mov dx, 51280
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB5
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB5:
   mov di, 176
   mov dx, 51600
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB6
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB6:
   mov di, 220
   mov dx, 51920
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB7
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB7:
   mov di, 264
   mov dx, 52240
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB8
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB8:
   mov di, 308
   mov dx, 52560
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB9
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB9:
   mov di, 352
   mov dx, 52880
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB10
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB10:
   mov di, 396
   mov dx, 53200
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCB11
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB

   loopCB11:
   mov di, 440
   mov dx, 53520
   cmp ax, dx
   jb loopCZB
   add dx, 41
   cmp ax, dx
   ja loopCZB
   sub dx, 41
   call temCor
   cmp cx, 1
   je houveChoqueZB
   jmp loopCZB

houveChoqueZB:
    push ax dx

    mov si, offset limpaTiroInimigo
    sub bx, 2
    mov tirosInimigos[bx], 0
    dec balas_na_tela
    sub ax, 1600
    mov bx, ax
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    pop dx ax
   ;mov dx, 50320
   sub ax, dx
   add ax, di
   mov bx, ax
   mov biro1[bx], 07h
   mov biro1[bx+1],07h
   mov biro1[bx+2], 07h
   mov biro1[bx+44], 07h
   mov biro1[bx+45], 07h
   mov biro1[bx+46], 07h
   call DesenhaBarreira


  fimCZB:
  pop di
  pop si ds
  popa
  ret
ChoqueZBarreira ENDP


;******************************************************
; Procedimento que verifica o choque do "Z" com a barreira2
;******************************************************
ChoqueZBarreira2  PROC NEAR
  pusha
  push ds si
  push di

  mov ax, @data
  mov ds, ax
  mov bx, 0

  loopCZB2:
   cmp bx, 24
   je fimCZB2
   mov ax, tirosInimigos[bx]    ; move pra ax a posicao do "Z"
   add bx, 2                    ; incrementa bx
   cmp ax, 0
   je loopCZB2

   add ax, 1600                 ; parte mais abaixo e mais a esquerda do "Z"
   mov di, 0
   mov dx, 50433
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB2:
   mov di, 44
   mov dx, 50753
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB3
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

  lCB3:
  mov di, 88
   mov dx, 51073
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB4
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

  lCB4:
  mov di, 132
   mov dx, 51393
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB5
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB5:
   mov di, 176
   mov dx, 51713
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB6
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB6:
   mov di, 220
   mov dx, 52033
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB7
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB7:
   mov di, 264
   mov dx, 52353
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB8
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB8:
   mov di, 308
   mov dx, 52673
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB9
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB9:
   mov di, 352
   mov dx, 52993
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB10
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB10:
   mov di, 396
   mov dx, 53313
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja lCB11
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2

   lCB11:
   mov di, 440
   mov dx, 53633
   cmp ax, dx
   jb loopCZB2
   add dx, 41
   cmp ax, dx
   ja loopCZB2
   sub dx, 41
   call temCor2
   cmp cx, 1
   je houveChoqueZB2
   jmp loopCZB2

houveChoqueZB2:
    push ax dx

    mov si, offset limpaTiroInimigo
    sub bx, 2
    mov tirosInimigos[bx], 0
    dec balas_na_tela
    sub ax, 1600
    mov bx, ax
    mov cx, 3
    mov dx, 6
    call PlotarFigura
    pop dx ax
   sub ax, dx
   add ax, di
   mov bx, ax
   mov biro2[bx], 07h
   mov biro2[bx+1],07h
   mov biro2[bx+2], 07h
   mov biro2[bx+44], 07h
   mov biro2[bx+45], 07h
   mov biro2[bx+46], 07h
   call DesenhaBarreira


  fimCZB2:
  pop di
  pop si ds
  popa
  ret
ChoqueZBarreira2 ENDP

;**********************************************
;Procedimento que verifica se a barreira1 jah foi destruida
;**********************************************
temCor PROC NEAR
   push ax bx
   ;mov dx, 50320
   sub ax, dx
   add ax, di
   mov bx, ax

   cmp biro1[bx], 07h
   jne houveChoqueTC
   cmp biro1[bx+1], 07h
   jne houveChoqueTC
   cmp biro1[bx+2], 07h
   jne houveChoqueTC
   mov cx, 0
   jmp fimTC

houveChoqueTC:
   mov cx, 1

   fimTC:
   pop bx ax
   ret

temCor ENDP

;**********************************************
;Procedimento que verifica se a barreira2 jah foi destruida
;**********************************************

temCor2 PROC NEAR
   push ax bx
   ;mov dx, 50320
   sub ax, dx
   add ax, di
   mov bx, ax

   cmp biro2[bx], 07h
   jne houveChoqueTC2
   cmp biro2[bx+1], 07h
   jne houveChoqueTC2
   cmp biro2[bx+2], 07h
   jne houveChoqueTC2
   mov cx, 0
   jmp fimTC2

houveChoqueTC2:
   mov cx, 1

   fimTC2:
   pop bx ax
   ret

temCor2 ENDP

;**********************************************
;Procedimento que verifica se o inimigo tah na 
;mesma "altura da barreira", se tiver,
; as barreiras somem
;**********************************************

choqueIniBar PROC NEAR
  pusha
  push ds si

  mov ax, @DATA
  mov ds, ax
  mov bx, 38

  loopCIB:
  cmp bx, -2
   je fimCIB
   mov ax, array_inimigo[bx]
   sub bx, 2
   cmp ax, -1
   je loopCIB
   add ax, posicaoInimigo
   add ax, 5440
   cmp ax, 50240
   jae apagaBar
   jmp loopCIB

apagaBar:
   mov dx, 0
   mov bx, 320
   div bx
   mov bx, 0
   sub ax, 157
   loopNumLinhas:
     cmp ax, -1
     je fimCIB
     dec ax
     mov cx, 0
     loopbar:
      cmp cx, 44
      je loopNumLinhas
      mov biro1[bx], 07h
      mov biro2[bx], 07h
      inc bx
      inc cx
      jmp loopBar


   fimCIB:
   call desenhaBarreira
  pop si ds
  popa
  ret
choqueIniBar ENDP

;**********************************************
;Procedimento que verifica se os inimigos tao 
; na mesma altura de Sergio, se tiver ele morre
;**********************************************

choqueIniProf PROC NEAR

  pusha
  push ds si

  mov ax, @DATA
  mov ds, ax
  mov bx, 38

  loopCIP:
  cmp bx, -2
   je fimCIP
   mov ax, array_inimigo[bx]
   sub bx, 2
   cmp ax, -1
   je loopCIP
   add ax, posicaoInimigo
   add ax, 5440
   cmp ax, 56640
   jae profMorreu
   jmp loopCIP

profMorreu:
   dec vidas
   mov sergioMorreu, 1
   cmp Vidas, -1
   jne fimCIP
   mov GameOver, 1
   call DesenhaVidas

fimCIP:
  pop si ds
  popa
  ret
choqueIniProf ENDP

;**********************************************
;Procedimento que desenha as vidas restantes na tela
;**********************************************

DesenhaVidas  PROC NEAR
  pusha
  push ds si di

  mov ax, @data
  mov ds, ax
  pusha
  mov si, offset strVidas
  mov dh, Crd
  mov di, 35    ; coluna
  mov cl, 10     ; linha
  mov bl, espera
  mov espera, 0
  push bx
  call DesenharFrase
  mov si, offset Branco
  mov bx, 2312
  mov cx, 39
  mov dx, 13
  call PlotarFigura
  pop bx
  mov espera, bl
  popa

  cmp Vidas, -1
  je fimDV
  cmp Vidas, 0
  je fimDV
  cmp Vidas, 1
  je desenha1Vida
  cmp Vidas, 2
  je desenha2Vidas
  cmp Vidas, 3
  je desenha3Vidas

  desenha1Vida:
    mov si, offset Pilot
    mov bx, 2970;3250
    mov cx, 3
    mov dx, 9
    call PlotarFigura

    mov si, offset LimpaPilot
    mov bx, 2976;3256
    call PlotarFigura

    mov bx, 2982;3262
    call PlotarFigura
    jmp fimDV

  desenha2Vidas:
    mov si, offset Pilot
    mov bx, 2967;3250
    mov cx, 3
    mov dx, 9
    call PlotarFigura

    mov bx, 2973;3256
    call PlotarFigura

    mov si, offset LimpaPilot
    mov bx, 2979;3262
    call PlotarFigura
    jmp fimDV

  desenha3Vidas:
    mov si, offset Pilot
    mov bx, 2964;3315
    mov cx, 3
    mov dx, 9
    call PlotarFigura

    mov bx, 2970;3321
    call PlotarFigura

    mov bx, 2976;3327
    call PlotarFigura
    jmp fimDV

  fimDV:
  pop di si ds
  popa
  ret
DesenhaVidas ENDP

;**********************************************
;Procedimento que verifica se houve choque entre
;a tampa e Emanoel
;**********************************************

ChoqueTampaEmanoel PROC NEAR
  pusha
  push ds si

  mov ax, @data
  mov ds, ax

  mov bx, posicao_mano
  mov dx, posicaoTampa
  ;sub dx, 320
  mov cx, 0

  lacoCTE:
    mov bx, posicao_mano
    cmp cx, 4
    je fimCTE
    inc cx
    mov ax, 320
    mul cx
    mov dx, posicaoTampa
    add dx, ax

  lacoChoqueTE:
    cmp mano_na_tela, 0
    je fimCTE

    add bx, 1280
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE2
    jmp houveChoqueTE

  cmpCTE2:
    mov bx, posicao_mano
    add bx, 1600
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE3
    jmp houveChoqueTE

  cmpCTE3:
    mov bx, posicao_mano
    add bx, 1920
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE4
    jmp houveChoqueTE

  cmpCTE4:
    mov bx, posicao_mano
    add bx, 2240
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE5
    jmp houveChoqueTE

  cmpCTE5:
    mov bx, posicao_mano
    add bx, 2560
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE6
    jmp houveChoqueTE

  cmpCTE6:
    mov bx, posicao_mano
    add bx, 2880
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja cmpCTE7
    jmp houveChoqueTE

  cmpCTE7:
    mov bx, posicao_mano
    add bx, 3200
    add dx, 2
    cmp dx, bx
    jb fimCTE

    add bx, 31
    sub dx, 2
    cmp dx, bx
    ja fimCTE ;lacoCTE
    jmp houveChoqueTE

  houveChoqueTE:
    push bx
    mov bx, 1000
    call PlaySound
    pop bx
    add Pontos, 30
    mov mano_na_tela, 0
    mov tampa_na_tela, 0
    mov si, offset LimpaEmanoel
    mov bx, posicao_mano
    mov cx, 34
    mov dx, 11
    call PlotarFigura

    mov si, offset limpaTampa
    mov bx, posicaoTampa
    mov cx, 3
    mov dx, 6
    mov posicaoTampa, 0
    call PlotarFigura
    call DesenhaScore
  fimCTE:
    pop si ds
    popa
    ret
ChoqueTampaEmanoel ENDP

;**********************************************
;Procedimento que desenha o Score atual na tela
;**********************************************

DesenhaScore   PROC NEAR
pusha
push di si
  mov si, offset strScore
  mov dh, Crd
  mov di, 230    ; coluna
  mov cl, 10     ; linha
  mov bl, espera
  mov espera, 0
  push bx
  call DesenharFrase
  mov si, offset Branco
  mov bx, 2508
  mov cx, 39
  mov dx, 13
  call PlotarFigura
  pop bx
  mov espera, bl

  mov ax, 0
  mov ax, Pontos

  mov bl, espera
  mov espera, 0

  push bx
  call ConverterDecimal

  mov di, 270
  mov ah, 10
  mov dh, Crd
  call DesenharNumero
  pop bx
  mov espera, bl
pop si di
popa
ret
DesenhaScore ENDP

;**********************************************
;Procedimento que verifica o nivel em que estah o jogo
;**********************************************
VerificaNivel PROC NEAR
  pusha
  mov ax, @data
  mov ds, ax
  cmp inimigos_vivos, 0
  jne fimVerificaN
  inc nivel
  fimVerificaN:
  popa
  ret
VerificaNivel ENDP

ProximoNivel PROC NEAR
 pusha
 popa
 ret
ProximoNivel ENDP

;*********************************************************
;
; procedimeto que inicializa as variaveis do jogo
;
;
;*********************************************************
Inicializar PROC NEAR
  mov totaltempo, 0
  mov totalscore, 0
  mov totalvidas, 600
  mov timeCounter, 0
  mov varPause, 0
  mov mudouFase, 0
  mov tampa_na_tela, 0
  mov posicao_tela, 57740       ; tem q recalcular o MEIO
  mov posicaoInimigo, 7132
  mov balas_na_tela, 0
  mov inimigos_vivos, 20
  mov direcaoInimigos, 1
  mov varEsc, 0
  mov cont, 30
  mov boolN, 0
  mov GameOver, 0
  mov SergioMorreu, 0

  mov mano_na_tela, 0
  mov mano_indo_volta, 0

  mov tirosInimigos[0], 0
  mov tirosInimigos[2], 0
  mov tirosInimigos[4], 0
  mov tirosInimigos[6], 0
  mov tirosInimigos[8], 0
  mov tirosInimigos[10], 0
  mov tirosInimigos[12], 0
  mov tirosInimigos[14], 0
  mov tirosInimigos[16], 0
  mov tirosInimigos[18], 0
  mov tirosInimigos[20], 0
  mov tirosInimigos[22], 0

  mov posicaoTampa, 0
  mov linha, 22
  mov var_esquerdo, 0
  mov var_direito, 133
  mov coluna1, 4
  mov coluna2, 4
  mov coluna3, 4
  mov coluna4, 4
  mov coluna5, 4
  mov inimigos_vivos, 20
  mov array_inimigo[0], 0
  mov array_inimigo[2], 30
  mov array_inimigo[4], 60
  mov array_inimigo[6], 90
  mov array_inimigo[8], 120
  mov array_inimigo[10], 6080
  mov array_inimigo[12], 6110
  mov array_inimigo[14], 6140
  mov array_inimigo[16], 6170
  mov array_inimigo[18], 6200
  mov array_inimigo[20], 12160
  mov array_inimigo[22], 12190
  mov array_inimigo[24], 12220
  mov array_inimigo[26], 12250
  mov array_inimigo[28], 12280
  mov array_inimigo[30], 18240
  mov array_inimigo[32], 18270
  mov array_inimigo[34], 18300
  mov array_inimigo[36], 18330
  mov array_inimigo[38], 18360
  call DesenhaFundo
  call DesenhaSergio
  call DesenhaInimigos
  call DesenhaBarreira
  ;call MoveTampa
  ret
Inicializar ENDP


;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x PROC Delay                                                                   x
;x                                                                              x
;x Descrição: chama o             x
;x            aproximadamente 15 microsegundos                                  x
;x                                                                              x
;x Parâmetros                                                                   x
;x [ds:espera] - recebe o tempo de espera que sera multiplicado por 100         x
;x               e somado por 900                                               x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
Delay      PROC NEAR
   pusha

   mov ax, @data
   mov ds, ax
   mov bh, espera
   cmp bh,0
   je fimDelay
   loopDelay:
   call CoolDown
;   call CoolDown

   dec bh
   cmp bh, 0
   jne loopDelay
   fimDelay:
   popa
   ret
Delay ENDP

;-------------------------------------------------------------------------------
; Função DesenharFrase
;
; Desenha uma frase na tela. A frase deve acabar com um byte 0.
; Se a frase contiver $ haverá uma quebra de linha e um retorno de carro
;
; Parâmetros:
; DS:SI - ponteiro para o início da frase
; DH - Cor para desenhar
; DI, CL - posição (X, Y) para desenhar
;-------------------------------------------------------------------------------
DesenharFrase		PROC NEAR

                        push di
                        push bx
                        mov bx, di

LerLetra:
           cmp podeSair, 1
              jne nsair
              cmp sair_hist, 1
              je FraseVoltar
           nsair:
           lodsb		; ler uma letra

			cmp al, 0	; byte 0 indica o fim da frase
			je FraseVoltar
			cmp al, ' '     ; detectado um espaço na frase
			je DarEspaco	; pular o desenho da letra
			                ; e dar soh o espaço
                        cmp  al, '$'
                        je PularLinha
			call DesenharLetra
			;jc LerLetra	; se o flag de carry estiver ligado,
			                ; algum simbolo não suportado foi
			                ; encontrado, pular ele



DarEspaco:
                        add di, 6
			jmp LerLetra

PularLinha:
                        add cl,10
                        mov di, bx
                        jmp LerLetra

FraseVoltar:
                        pop bx
                        pop di

                        RET

DesenharFrase		ENDP
;-------------------------------------------------------------------------------
; Fim da Função DesenharFrase
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; Funcao DesenharLetra
;
; Desenha uma letra na tela
;
; Parâmetros:
; AL - Letra para Desenhar (codigo ascii)
; DH - Cor para desenhar
; DI, CL - Localizacao (X, Y)
;
; Retorno:
; Flag de carry será ativado se a função não terminar corretamente (código
; passado não é uma letra)
;
; Observações:
; Se o codigo especificado não for de uma letra valida, nada será desenhado.
;-------------------------------------------------------------------------------
DesenharLetra		PROC NEAR

                        push bx
                        push ds
                        push si
                        push ax

			stc
			mov bx, ax
			mov ax, seg Letras
			mov ds, ax
			LEA si, Letras	; colocar o ponteiro no inicio dos sprites de letras
			mov ax, bx
                        cmp al, ','
                        je Virgula
                        cmp al, '-'
                        je Traco
                        cmp al, '!'
                        je Exclamacao
                        cmp al, '.'
                        je Ponto
                        cmp al, '?'
                        je Interrogacao
                        cmp al, 'á'
                        je a_agudo
                        cmp al, 'â'
                        je a_vovo
                        cmp al, 'ã'
                        je a_til
                        cmp al, ':'
                        je DoisPontos
                        cmp al, 'í'
                        je i_agudo
                        cmp al, 'é'
                        je e_agudo
                        cmp al, 'ê'
                        je e_vovo
                        cmp al, 'ç'
                        je cedilha
                        cmp al, 20h
                        je espaco_                        
                       	cmp al, '0'
                       	je numero0
                       	cmp al, '1'
                       	je numero1
                       	cmp al, '2'
                       	je numero2
                       	cmp al, '3'
                       	je numero3
                       	cmp al, '4'
                       	je numero4
                       	cmp al, '5'
                       	je numero5
                       	cmp al, '6'
                       	je numero6
                       	cmp al, '7'
                       	je numero7
                       	cmp al, '8'
                       	je numero8
                       	cmp al, '9'
                       	je numero9
                       	cmp al, '('
                       	je parDir
                       	cmp al, ')'
                       	je parEsq
                       	cmp al, '/'
                       	je barra
       		stc
            test al, 20h	; verificar letra minuscula
			jz maiuscula
			add si, 260	; pular para o 'a' minusculo
			and al, 0dfh	; desativar o bit de minuscula
maiuscula:
                        cmp al, 'A'
			jb DesenharLetraRet
			cmp al, 'Z'
			jg DesenharLetraRet
			sub al, 'A'	; retirar o codigo ascii
			xor ah, ah	; multiplicar por 10 (numero de bytes
			shl ax, 1	; por sprite de letra)
			mov bx, ax	;
			shl ax, 1	;
			shl ax, 1	; multiplicacao terminada
			add bx, ax
			add si, bx	; agora esta na letra (sprite) certa
			push di
			mov al, cl	; conversao de parametros - al esta com a posicao Y
			push cx
			mov ah, 6	; numero de colunas
			mov dl, 10	; numero de linhas
			call DesenharSprite
			pop cx
			pop di
			clc


DesenharLetraRet:
                        cmp al, '-'
                        je Virgula
                        jmp final
Virgula:
			add si, 520	; agora esta na letra (sprite) certa
			mov al, cl	; conversao de parametros - al esta com a posicao Y
			mov ah, 6	; numero de colunas
			mov dl, 10	; numero de linhas
			call DesenharSprite
                        jmp final
Traco:
                        add si, 560	; agora esta na letra (sprite) certa
			mov al, cl	; conversao de parametros - al esta com a posicao Y
			mov ah, 6	; numero de colunas
			mov dl, 10	; numero de linhas
			call DesenharSprite
                        jmp final

Exclamacao:
                        add si, 550	; agora esta na letra (sprite) certa
			mov al, cl	; conversao de parametros - al esta com a posicao Y
			mov ah, 6	; numero de colunas
			mov dl, 10	; numero de linhas
			call DesenharSprite
                        jmp final
Ponto:
                        add si, 570	; agora esta na letra (sprite) certa
                        jmp Economiza
Interrogacao:
                        add si, 530	; agora esta na letra (sprite) certa
                        jmp Economiza
DoisPontos:
                        add si, 540	; agora esta na letra (sprite) certa
                        jmp Economiza
a_agudo:
                        add si, 580	; agora esta na letra (sprite) certa
                        jmp Economiza
a_til:
                        add si, 590	; agora esta na letra (sprite) certa
                        jmp Economiza
a_vovo:
                        add si, 600     ; agora esta na letra (sprite) certa
                        jmp Economiza
i_agudo:
                        add si, 610     ; agora esta na letra (sprite) certa
                        jmp Economiza
cedilha:
                        add si, 620     ; agora esta na letra (sprite) certa
                        jmp Economiza
e_agudo:
                        add si, 630     ; agora esta na letra (sprite) certa
                        jmp Economiza
e_vovo:
                        add si, 640     ; agora esta na letra (sprite) certa
                        jmp Economiza
espaco_:
                        add si, 650     ; agora esta na letra (sprite) certa
                        jmp Economiza

numero0:
                        add si, 660     ; agora esta na letra (sprite) certa
                        jmp Economiza

numero1:
                        add si, 670     ; agora esta na letra (sprite) certa
                        jmp Economiza

numero2:
                        add si, 680     ; agora esta na letra (sprite) certa
                        jmp Economiza
numero3:
                        add si, 690     ; agora esta na letra (sprite) certa
                        jmp Economiza
numero4:
                        add si, 700     ; agora esta na letra (sprite) certa
                        jmp Economiza
numero5:
                        add si, 710     ; agora esta na letra (sprite) certa
                        jmp Economiza

numero6:
                        add si, 720     ; agora esta na letra (sprite) certa
                        jmp Economiza

numero7:
                        add si, 730     ; agora esta na letra (sprite) certa
                        jmp Economiza
numero8:
                        add si, 740     ; agora esta na letra (sprite) certa
                        jmp Economiza
numero9:
                        add si, 750     ; agora esta na letra (sprite) certa
                        jmp Economiza

parDIr:
                        add si, 760     ; agora esta na letra (sprite) certa
                        jmp Economiza
parEsq:
                        add si, 770     ; agora esta na letra (sprite) certa
                        jmp Economiza
barra:
                        add si, 780     ; agora esta na letra (sprite) certa
                        jmp Economiza


Economiza:
          		mov al, cl	; conversao de parametros - al esta com a posicao Y
			mov ah, 6	; numero de colunas
			mov dl, 10	; numero de linhas
			call DesenharSprite
                        jmp final
Final:
                        pop ax
			pop si
			pop ds
			pop bx

                        RET

DesenharLetra		ENDP

;-------------------------------------------------------------------------------
; Fim Da Funcao DesenharLetra
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Funcao Desenhar Sprite
;
; Desenha um Sprite qualquer na tela
;
; Parâmetros:
; DS:SI tem que apontar para o inicio dos desenhos do sprite
; DL - Numero de Linhas
; AH - Numero de Colunas
; DH - Cor para desenhar
; DI, AL - Localizacao (X, Y) (320x240)
;-------------------------------------------------------------------------------
DesenharSprite		PROC NEAR

                        push cx
                        push ax
                        push bx
                        push di
                        push es
                        push si
                        push dx

; Calcular Posicao e Colocar em DI
			mov ch, ah	; ch agora esta com o numero de colunas
			mov AH,0
			SHL AX,1	; esses shifts todos multiplicam o ax
			SHL AX,1	; por 320, ou seja, calculam o valor na
			SHL AX,1	; memoria de video do inicio da linha
			SHL AX,1	;
			SHL AX,1	;
			SHL AX,1	;
			mov BX, AX
			SHL AX,1	;
			SHL AX,1	;
			add BX, AX	; somar com o DI (X) para ter o endereco
			add DI, BX	; correto
			mov AX,	0a000h; acessar a memoria de video
			mov ES, AX
			mov ah, ch	; ah fica como "constante"
			mov ch, 0
;Desenhar Linha a Linha			; para poder usar o ch como contador
DesenharLinha:		mov bh, ah	; bh tem o numero de colunas restantes
DesenharByte:		LODSB 		; AL contem a linha a ser desenhada
			mov bl, al	; preciso usar o al para enviar para o video
			mov al, dh	;
			cmp bh, 8
			jg ColunaDemais
ColunaDemenos:		mov cl, bh
                        jmp DesenharGrupo
ColunaDemais:           mov cl, 8
DesenharGrupo:		sub bh, cl
DesenharBit:		test bl, 80h	; testar o bit mais significativo
			jz NaoDesenhar	; nao desenhar se o bit for 0
			stosb		; store byte
			jmp Desenhado	; pular o eskema do 0 (DI jah incrementado)
NaoDesenhar:		inc DI		; stosb nao foi executado, passar para o proximo pixel
Desenhado:		shl bl, 1	; deslocar o bl para o proximo bit
			loop DesenharBit; passar para o proximo bit
			cmp bh, 0
			jnz DesenharByte
			add di, 320	; posicionar para a proxima linha
			mov al, ah
			mov ah, 0
			sub di, ax	; posicionamento concluido
			mov ah, al
			dec dl
			jnz DesenharLinha; repetir ateh acabar as linhas
                                                ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                        call Delay
                        pop dx
                        pop si
                        pop es
                        pop di
                        pop bx
                        pop ax
                        pop cx

			RET

DesenharSprite		ENDP

;-------------------------------------------------------------------------------
; Fim Da Funcao DesenharSprite
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Funcao DesenharNumero
;
; Desenha um numero com base decimal em um ponto da tela.
;
; Parâmetros:
; BH - Digito mais significativo
; BL
; CH
; CL
; DL - Digito menos signiticativo
; DH - Cor para desenhar
; DI, AH - Localizacao (X, Y) (320x240)
;-------------------------------------------------------------------------------
DesenharNumero          PROC NEAR

                        push ax
                        push cx

                        mov al, bh
                        push cx
                        mov cl, ah
                        call DesenharDigito
                        pop cx
                        add di, 7
                        mov al, bl
                        push cx
                        mov cl, ah
                        call DesenharDigito
                        pop cx
                        add di, 7
                        mov al, ch
                        push cx
                        mov cl, ah
                        call DesenharDigito
                        pop cx
                        add di, 7
                        mov al, cl
                        mov cl, ah
                        call DesenharDigito
                        add di, 7
                        mov al, dl
                        mov cl, ah
                        call DesenharDigito

                        pop cx
                        pop ax
                        RET

DesenharNumero          ENDP
;-------------------------------------------------------------------------------
; Fim da Funcao DesenharNumero
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Funcao DesenharDigito
;
; AL - Numero para Desenhar
; DH - Cor para desenhar
; DI, CL - Localizacao (X, Y)
;
; Retorno:
; Flag de carry será ativado se a função não terminar corretamente (código
; passado não é uma letra)
;
; Observações:
; Se o codigo especificado não for de uma letra valida, nada será desenhado.
;-------------------------------------------------------------------------------
DesenharDigito		PROC NEAR

                        push bx
                        push ds
                        push si
                        push ax
                        push dx

			stc
			mov si, seg Letras
			mov ds, si
			LEA si, Numeros	; colocar o ponteiro no inicio dos sprites de numeros
                        cmp al, 10
                        jg DesenharDigitoRet
			xor ah, ah	; multiplicar por 8
			mov bx, ax
			shl ax, 3	;
			sub ax, bx      ; diminuir o numero (resultado: ax = ax * 7)
			add si, ax	; agora esta no numero (sprite) certo
			push di
			mov al, cl	; conversao de parametros - al esta com a posicao Y
			push cx
			mov ah, 6	; numero de colunas
			mov dl, 7	; numero de linhas
			call DesenharSprite
			pop cx
			pop di
			clc


DesenharDigitoRet:      pop dx
			pop ax
			pop si
			pop ds
			pop bx

                        RET

DesenharDigito          ENDP

;-------------------------------------------------------------------------------
; Fim da Funcao DesenharDigito
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Funcao Setarint15
;
; Substitui a Int 15h inicial pelo KeyboardHandler e guarda o segmento e offset
; nas variaveis OldInt15_SEG e OldInt15_OFFSET.
;-------------------------------------------------------------------------------
SetarInt15              PROC NEAR

                        push ax
                        push es
                        push bx
                        push ds
                        push dx

                        mov ax, @data
                        mov ds, ax

			mov ax, 3515h
			int 21h
                        mov [OldInt15_SEG], es
                        mov [OldInt15_OFFSET], bx
			mov ax, seg KeyboardHandler
			mov ds, ax
			mov dx, offset KeyboardHandler
			mov ax, 2515h
			int 21h

			pop dx
			pop ds
			pop bx
			pop es
			pop ax

                        RET

SetarInt15              ENDP
;-------------------------------------------------------------------------------
; Fim da Funcao SetarInt15
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Funcao ResetarInt15
;
; Registra a Int 15 antiga, conforme o segmento e offset armazenados em
; OldInt15_SEG e OldInt15_OFFSET
;-------------------------------------------------------------------------------
ResetarInt15		PROC NEAR

			push ax
			push ds
			push dx

                        mov ax, @data
                        mov ds, ax

			mov dx, [OldInt15_OFFSET]
                        mov ax, [OldInt15_SEG]
			mov ds, ax
			mov ax, 2515h
			int 21h

			pop dx
			pop ds
			pop ax

			RET

ResetarInt15		ENDP
;-------------------------------------------------------------------------------
; Fim da Funcao ResetarInt15
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; KeyboardHandler
;
; Substitui a interrupcao 15h antiga para podermos tratar o teclado do jeito que
; quisermos.
;-------------------------------------------------------------------------------
KeyboardHandler		PROC FAR

                        cli

                        push ds
                        push bx
                        push ax

                        mov bx, @data
                        mov ds, bx

			cmp ah, 04fh		; eh pra eu tratar?
			jne naoTratar

			test al, 80h            ; é evento de tecla pressionada?
			jnz PressionarDireita
			mov [PressionouTecla], 1
			mov sair_hist, 1

PressionarDireita:	cmp al, 04Dh
                        jne SoltarDireita
                        mov ds:[varRight], 1
                        jmp KeyboardHandlerVoltar

SoltarDireita:          cmp al, 0CDh
                        jne PressionarEsquerda
                        mov ds:[varRight], 0
                        jmp KeyboardHandlerVoltar

PressionarEsquerda:     cmp al, 04Bh
                        jne SoltarEsquerda
                        mov ds:[varLeft], 1
                        jmp KeyboardHandlerVoltar

SoltarEsquerda:         cmp al, 0CBh
                        jne PressionarP
                        mov ds:[varLeft], 0
                        jmp KeyboardHandlerVoltar


PressionarP:            cmp al, 019h
                        jne PressionarEspaco
                        cmp ds:[varPause], 1
                        jne togglerP
                        mov ds:[varPause], 0
                        jmp KeyboardHandlerVoltar
                        togglerP:
                        mov ds:[varPause], 1
                        jmp KeyboardHandlerVoltar


PressionarEspaco:       cmp al, 39h
                        jne PressionarESC
                        cmp tampa_na_tela, 1
                        je KeyboardHandlerVoltar
                        mov ds:[varEspaco], 1              ;sera zerado quando sair da tela
                        jmp KeyboardHandlerVoltar


PressionarESC:          cmp al, 001h
                        jne PressionarS
                        mov ds:[varEsc], 1
                        jmp KeyboardHandlerVoltar

PressionarS:            cmp al, 1Fh
                        jne PressionarN
                        mov ds:[varS], 1
                        jmp KeyboardHandlerVoltar

PressionarN:            cmp al, 31h
                        jne PressionarM
                        mov ds:[varN], 1
                        jmp KeyboardHandlerVoltar
PressionarM:    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;Emanoel
                        cmp al, 32h
                        jne PressionarF2
                        push ax
                        mov ah, 1
                        sub ah, habilitaSom
                        mov habilitaSom, ah
                        pop ax
                        jmp KeyboardHandlerVoltar      ;;;;;;;;;;;;;;;;;;;;Emanoel

PressionarF2:           cmp al, 3Ch
                        jne naoTratar
                        cmp ds:[nivel], 4
                        jb incrementaNivel
                        mov ds:[nivel], 0
                        incrementaNivel:
                        inc ds:[nivel]
                        jmp KeyboardHandlerVoltar

;*********************************************************
;
;se ficar meio lento modificar o acesso a mem em baixo
;
;*********************************************************


naoTratar:
                        push ax
                        push es
                        push di
                        push si

                        mov ax,40h
                        mov es,ax
                        mov ds,ax
                        mov di,1ch
                        mov si,1ah
                        movsw

                        pop si
                        pop di
                        pop es
		                pop ax

			            clc
KeyboardHandlerVoltar:
                        pop ax
                        pop bx
                        pop ds

			sti

			IRET

KeyboardHandler		ENDP
;-------------------------------------------------------------------------------
; Fim do KeyboardHandler
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Funcao EsperarTecla
;
; Espera o usuário apertar alguma tecla e retorna.
;-------------------------------------------------------------------------------
EsperarTecla		PROC NEAR

		push ax
		push ds

		mov ax, @data
		mov ds, ax

        mov [PressionouTecla], 0

EsperarTeclaLoop:
                  cmp [PressionouTecla], 0
			      je EsperarTeclaLoop

        pop ds
        pop ax

        RET

EsperarTecla		ENDP

;-------------------------------------------------------------------------------
; Fim da Funcao EsperarTecla
;-------------------------------------------------------------------------------



;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x PROC CoolDown                                                                x
;x                                                                              x
;x Descrição: Espera o canhão do monitor voltar a posição 0                     x
;x            É necessário sempre chamar essa função quando se for carregar uma x
;x            figura na tela quando já se tem outra.                            x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

CoolDown 	PROC	NEAR

;Empilha dx, ax
push dx ax

;Porta do registrador de status do monitor
mov dx, 03DAh

;Espera o bit 3 zerar (periodo ativo)
loop1:
  in   al, dx
  test al, 8
  jnz  loop1

;Espera o bit 3 setar (periodo de retraco)

loop2:
  in   al, dx
  test al, 8
  jz   loop2

;Desempilha ax, dx
pop ax dx
ret

CoolDown    ENDP

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x PROC GeraNumeroAleatorio                                                     x
;x                                                                              x
;x Descrição: Gera um numero (pseudo-)aleatorio entre 0 e X                     x
;x            (baseado em congruencia linear).                                  x
;x Parâmetros:                                                                  x
;x   DX      Teto do número aleatório                                           x                                                                           x
;x                                                                              x
;x Retorno                                                                      x
;x   BX      Número entre 0 e DX                                                x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

GeraNumeroAleatorio   PROC    NEAR

;Empilha registradores
push ax cx dx

;Multiplica a semente pelo fator e soma a constante
mov ax, [ds:SeeD]
mov bx, [ds:factor]
mul bx
add ax, 1

;Obtem o resto da divisao por 64 (o modulo) e armazena na memória

;and ax, 3Fh
mov [ds:SeeD], ax
xor dx, dx

pop dx
;Obtem o resto (modulo) da divisao DX:AX / 40
mov bx, dx
mov dx,0
div bx

;Joga o numero para BX
mov bx, dx

;Desempilha registradores
pop cx ax
ret

;Fim do procedimento GeraNumeroAleatorio
GeraNumeroAleatorio   ENDP

;-------------------------------------------------------------------------------
; Funcao ConverterDecimal
;
; Parâmetros:
; AX - contem o numero
;
; Retorno :
;
; BH - mais significativo
; BL
; CH
; CL
; DL - menos significativo
;
;-------------------------------------------------------------------------------
ConverterDecimal	PROC NEAR

                        push ax
                        push di

                        xor dx, dx
                        mov di, 10000
                        div di
                        mov bh, al        ; Quociente
                        mov ax, dx        ; Resto

                        xor dx, dx
                        mov di, 1000
                        div di
                        mov bl, al        ; Quociente
                        mov ax, dx        ; Resto

                        xor dx, dx
                        mov di, 100
                        div di
                        mov ch, al        ; Quociente
                        mov ax, dx        ; Resto

                        xor dx, dx
                        mov di, 10
                        div di
                        mov cl, al        ; Quociente em CL e Resto em DL


                        pop di
                        pop ax

                        RET

ConverterDecimal       ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;Emanoel
;--------------------------------------------------------------------------------
; Proc PlaySound
; Executa um Som
;
; Parametros
;   BX      Frequencia
;--------------------------------------------------------------------------------
    PlaySound   PROC     NEAR
        ; empilha registradores
        pusha
        mov ax, @data
        mov ds, ax
        ; verifica se o som esta habilitado
        cmp [ds:habilitaSom], 0
        je  PSDone

        ; frequencia do PIT
        mov  ax, 34DDh
        mov  dx, 0012h

        ; verifica se a frequencia esta no intervalo permitido
        cmp  dx, bx
        jnc  PSDone

        ; contador do PIT
        div  bx
        mov  bx, ax

        ; obtem status do PC Speaker
        in   al, 61h

        ; verifica se o som esta habilitado e atrelado ao PIT
        test al, 3
        jnz  PSLoop

        ; habilita o som e atrela ao PIT
        or   al, 3
        out  61h, al

        ; controle do PIT (modifica contador)
        mov  al, 0B6h
        out  43h, al

    PSLoop:
        ; envia valores do contador ao PIT
        mov al, bl
        out 42h, al
        mov al, bh
        out 42h, al

    PSDone:
        ; desempilha registradores
        popa
        ret
    PlaySound   ENDP

;--------------------------------------------------------------------------------
; Proc NoSound
; Desliga o Som
;--------------------------------------------------------------------------------
    NoSound   PROC     NEAR
        ; empilha AX
        push ax

        ; desabilita o PC Speaker (Zera os Bits 0 e 1)
        in      al, 061h
        and     al, 0FCh
        out     61h, al

        ; desempilha AX
        pop ax
        ret
    NoSound   ENDP

;--------------------------------------------------------------------------------
; Proc GamePlaySound
; Executa o som por determinado tempo
; bx: frequencia do som
; dh:duracao do som
;--------------------------------------------------------------------------------
    MenuPlaySound   PROC    NEAR
        ; empilha registradores
        pusha
        ; executa o som
       call PlaySound
  mov espera, dh
 call Delay
        call NoSound
        ; desempilha registradores
        popa
        ret
    MenuPlaySound   ENDP

;---------------------------------------------------------------------------------
; SoundMorreu
; Faz o som para a morte de Sergio.(Bixinho)
;
;---------------------------------------------------------------------------------
SoundMorreu PROC NEAR
  pusha
  mov ah, espera
  mov espera,6

  mov bx, 1000
  call PlaySound
  call Delay

  mov bx, 800
  call PlaySound
  call Delay

  mov bx, 600
  call PlaySound
  call Delay

  mov bx, 400
  call PlaySound
  call Delay

  mov bx, 200
  call PlaySound
  call Delay

  mov bx, 50
  call PlaySound
  mov espera,8

  call Delay

  call NoSound
  mov espera, ah
  popa
  ret
SoundMorreu ENDP


;**********************************************
;Procedimento que atualiza a variavel que guarda
; o tempo
;**********************************************
ContaTempo PROC NEAR
pusha
push ds es
mov ax, @data
mov ds, ax
mov ax,40h
mov es, ax

mov ax, es:[6ch]
cmp tempoRTC, ax
je fimTempo
inc contadorRTC
cmp contadorRTC, 18
je incTimeCounter
jmp fimTempo
incTimeCounter:
inc timeCounter
mov contadorRTC,0
fimTempo:
mov tempoRTC, ax
pop es ds
popa
ret
ContaTempo ENDP

;**********************************************
;Procedimento que desenha o tempo decorrido na tela
;**********************************************

DesenhaTempo  PROC NEAR
pusha
push ds di
mov ax, @data
mov ds, ax

;;;;
pusha
mov si, offset Branco
mov bx, 2390
mov cx, 39
mov dx, 13
call PlotarFigura
popa
;;;;;;
pusha
push ax
mov ax, @data
mov ds, ax
pop ax
mov ax, timeCounter
mov dl, espera
mov espera, 0
push dx
call ConverterDecimal
  mov di, 152
  mov ah, 10
  mov dh, Crd
  call DesenharNumero
  pop dx
mov espera, dl
  popa
pop di ds
popa
ret
DesenhaTempo ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;Emanoel
