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

CriaMenu PROC NEAR
  pusha
  push es ds
  mov ax,@data
  mov ds, ax                 ;faz DS apontar para o segmento de dados
  mov ah,00h
  int 1Ah
  mov [ds:SeeD],dx
  mov cx, 0                  ;inicializa o cx com o valor da tela do menuInicar
  call InicializaMenu        ; chama o proc InicializaMenu que vai desenhar o menu na tela

inicio:
  ;*** lê a tecla pressionada ***
  mov ax , 0800h
  int 21h
  cmp al, 6Dh
  je anulaSom
  ;*** verifica qual tecla foi pressionada ***
  ;cmp al, tesc
  ;je fim
  cmp al, up
   je cima
  cmp al, down
   je baixo
  cmp al, tenter
   je tecla_enter
  jmp inicio
;Desabilita o som
anulaSom:
      push ax
      mov ah, 1
      sub ah, habilitaSom
      mov habilitaSom, ah
      pop ax
      jmp inicio


;*******************************************************
;
;Inicio da rotina que trata a tecla ENTER
;
;*******************************************************
tecla_enter:
  ;*** Carrega o item do menu selecionado quando o ENTER é pressionado***
  cmp cx, 00h
   je tela_iniciar
  cmp cx, 01h
   je tela_recordes
  cmp cx, 02h
   je tela_creditos
  cmp cx, 03h
   je  tela_ajuda
  cmp cx, 04h
   je tela_sair

tela_iniciar:
;*** inicializa a tela do Jogo
;primeira tela
  mov dx, offset telahist1_pal
  call CarregaPaleta
  mov dx, offset telahist1_dat
  call DesenhaTela
;call LimpaTela
  push cx
  mov ax, @data
  mov ds, ax
  mov si, offset historinha1
  mov dh, 0EAh
  mov di, 5
  mov cl, 90
  call SetarInt15
  mov podeSair, 1
  mov sair_hist, 0
  mov ds:[espera], 5
  call DesenharFrase
  cmp sair_hist, 1
  je pula_hist1
  ;espera precionar alguma tecla entre a primeira e a segunda tela
  call ResetarInt15
  mov ax, 0800h
  int 21h
  call SetarInt15
  pula_hist1:
  mov sair_hist, 0
  ;segunda tela
  mov dx, offset telahist1_pal
  call CarregaPaleta
  mov dx, offset telahist1_dat
  call DesenhaTela
  mov si, offset historinha2
  mov dh, 0EAh
  mov di, 5
  mov cl, 100
  mov sair_hist, 0
  mov ds:[espera], 5

  call DesenharFrase
  cmp sair_hist, 1
  je pula_hist2
  ;espera precionar alguma tecla entre a segunda e a terceira tela
  call esperarTecla
  pula_hist2:
  mov sair_hist, 0
  ;terceira tela
  mov dx, offset telahist2_pal
  call CarregaPaleta
  mov dx, offset telahist2_dat
  call DesenhaTela
  mov espera, 5
  call delay

  mov dx, offset telahist3_pal
  call CarregaPaleta
  mov dx, offset telahist3_dat
  call DesenhaTela

  call delay
  mov dx, offset telahist4_pal
  call CarregaPaleta
  mov dx, offset telahist4_dat
  call DesenhaTela
  mov si, offset historinha3

  mov dh, 0EAh
  mov di, 70
  mov cl, 125
  mov sair_hist, 0
  mov ds:[espera], 5

  call DesenharFrase
  
  cmp sair_hist, 1
  je saiu_hist
  jmp pulinho
  saiu_hist:
  mov podeSair, 0
  call ResetarInt15
  jmp pulinho2
  pulinho:
  mov podeSair, 0
  call ResetarInt15
  mov ax, 0800h                     ;interrupcao de teclado
  int 21h
  pulinho2:
  mov ds:[varEsc], 0               ;sai depois que for pressionada uma tecla
;*** Começa o jogo
  pop cx

  call SetarInt15
  call Inicializar
  call JogarMesmo
 ; cli
  call ResetarInt15
  call LeScore                     ; alteracoes de Martinelli
  call CheckHighScore              ;
  call EscreveScore                ;
 ; sti
  mov cx, 0h
  call InicializaMenu

jmp inicio

tela_recordes:
  ;;; aqui !!! ;;;
  pusha
  push ds es di


  mov cx, 1h                     ;carrega em cx o valor da tela MenuRecordes
  mov dx, offset telarecordes_pal
  call CarregaPaleta
  mov dx, offset telarecordes_dat
  call DesenhaTela

  ;;;!!!Alteracao
  ;call LimpaTela
  call LeScore
  call imprimeTodos

  ;;;!!!Alteracao


  mov ax, 0800h                     ;interrupcao de teclado
  int 21h                           ;sai depois que for pressionada uma tecla
  call InicializaMenu

  pop di es ds
  popa
  jmp inicio
  
tela_Creditos:
  mov cx, 2h                        ;carrega em cx o valor da tela MenuCreditos
  call ProcTelaCredito
  ;call LeScore                   ;colocar logo q inicializa o jogo, se quizer
  ;mov Pontos, 200
  ;call CheckHighScore
  ;call EscreveScore
  ;call PegaNome
  call InicializaMenu               ;volta para o menu, mas apontando para "Creditos"
  jmp inicio
;*************************
tela_ajuda:
  mov cx, 3h                        ;carrega em cx o valor da tela MenuAjuda
  mov dx, offset telaajuda_pal
  call CarregaPaleta
  mov dx, offset telaajuda_dat
  call DesenhaTela
  mov ax, 0800h                     ;interrupcao de teclado
  int 21h                           ;sai depois que for pressionada uma tecla
  call InicializaMenu
  jmp inicio
;*************************
tela_Sair:
                                    ; carrega a tela que pergunta se quer sair realmente
  mov dx, offset telasair_pal
  call CarregaPaleta
  mov dx, offset telasair_dat
  call DesenhaTela
loopSair:
  mov ax, 0000h                     ; interrupcao de teclado
  int 16h
  cmp al,'S'                        ; verifica a tecla pressionada
  je apertou_s
  cmp al,'s'
  je apertou_s
  cmp al,'N'
  je apertou_n
  cmp al,'n'
  je apertou_n
  jmp loopSair
apertou_s:
  mov dx, offset telasaindo_pal     ; começa a animação carregando a tela "Pronto"
  call CarregaPaleta
  mov dx, offset telasaindo_dat
  call DesenhaTela
  mov espera, 50
  call  Delay
  mov dx, offset telasaiu_pal       ; carrega a tela "Rodando"
  call CarregaPaleta
  mov dx, offset telasaiu_dat
  call DesenhaTela
  call Delay
  jmp fim                           ;sai do Jogo
apertou_n:
  mov cx, 04h
  call InicializaMenu
  jmp inicio
;*******************************************************
;
;Fim da rotina que trata a tecla ENTER
;
;*******************************************************

cima:
  mov bx, 1000
  mov dh, 2
  call MenuPlaySound
  cmp cx, 00h             ;se tiver apontando para o "Iniciar", desce para o "Sair"
   je Menu_Sair
  cmp cx, 01h             ;se tiver apontando para o "Recordes", sobe para o "Iniciar"
   je Menu_Iniciar
  cmp cx, 02h             ;se tiver apontando para o "Creditos", sobe para o "Recordes"
   je Menu_Recordes
  cmp cx, 03h             ;se tiver apontando para o "Ajuda", sobe para o "Creditos"
   je Menu_Creditos
  cmp cx, 04h             ;se tiver apontando para o "Sair", sobe para o "Ajuda"
   je Menu_Ajuda
;*************************
baixo:
    mov bx, 1000
  mov dh, 2
  call MenuPlaySound
  cmp cx, 00h             ;se tiver apontando para o "Iniciar", desce para o "Recordes"
  je Menu_Recordes
  cmp cx, 01h             ;se tiver apontando para o "Recordes", desce para o "Creditos"
  je Menu_Creditos
  cmp cx, 02h             ;se tiver apontando para o "Creditos", desce para o "Ajuda"
  je Menu_Ajuda
  cmp cx, 03h             ;se tiver apontando para o "Ajuda", desce para o "Sair"
  je Menu_Sair
  cmp cx, 04h             ;se tiver apontando para o "Sair", sobe para o "Iniciar"
  je Menu_Iniciar
  jmp inicio
;*************************
Menu_Iniciar:              ; carrega a tela MenuIniciar
  mov cx,00h
  mov dx,offset menuIniciar_pal
  call CarregaPaleta
  mov dx,offset menuIniciar_dat
  call DesenhaTela
  jmp inicio
;*************************
Menu_Recordes:             ; carrega a tela MenuRecordes
  mov cx,01h
  mov dx,offset menuRecordes_pal
  call CarregaPaleta
  mov dx,offset menuRecordes_dat
  call DesenhaTela
  jmp inicio
;*************************
Menu_Creditos:             ; carrega a tela MenuCreditos
  mov cx,02h
  mov dx,offset menuCreditos_pal
  call CarregaPaleta
  mov dx,offset menuCreditos_dat
  call DesenhaTela
  jmp inicio
;*************************
Menu_Ajuda:                ; carrega a tela MenuAjuda
  mov cx,03h
  mov dx,offset menuAjuda_pal
  call CarregaPaleta
  mov dx,offset menuAjuda_dat
  call DesenhaTela
  jmp inicio
;*************************
Menu_Sair:                 ; carrega a tela MenuSair
  mov cx,04h
  mov dx,offset menuSair_pal
  call CarregaPaleta
  mov dx,offset menuSair_dat
  call DesenhaTela
  jmp inicio
;*************************
fim:
pop ds es
popa
ret
CriaMenu ENDP


;****************************************
; Procedimento que chama o menu Principal
; restaurando a posicao para a qual o piloto
; tava apontando antes
;****************************************
InicializaMenu PROC NEAR

  push ax es dx

  mov ax, 0A00h
  mov es, ax
  cmp cx, 00h
  je menuIni
  cmp cx, 01h
  je menuRec
  cmp cx, 02h
  je menuCred
  cmp cx, 03h
  je menuAju
  cmp cx, 04h
  je menuSai

menuIni:
  mov dx,offset menuIniciar_pal
  call CarregaPaleta
  mov dx,offset menuIniciar_dat
  call DesenhaTela
  jmp retornar

menuRec:
  mov dx,offset menuRecordes_pal
  call CarregaPaleta
  mov dx,offset menuRecordes_dat
  call DesenhaTela
  jmp retornar

menuCred:
  mov dx,offset menuCreditos_pal
  call CarregaPaleta
  mov dx,offset menuCreditos_dat
  call DesenhaTela
  jmp retornar

menuAju:
  mov dx,offset menuAjuda_pal
  call CarregaPaleta
  mov dx,offset menuAjuda_dat
  call DesenhaTela
  jmp retornar

menuSai:
  mov dx,offset menuSair_pal
  call CarregaPaleta
  mov dx,offset menuSair_dat
  call DesenhaTela
  jmp retornar

retornar:
  pop dx es ax
  ret
InicializaMenu ENDP
;*********Fim do Procedimento InicializaMenu**********

;****************************************
; Procedimento que carrega a tela de creditos
; e verifica a tecla pressionada para mudança 
; nas telas
;****************************************
ProcTelaCredito PROC NEAR
  push bx dx ax
  ;****************************
  telacred1:
    mov dx,offset telacredito_pal
    call CarregaPaleta
    mov dx,offset telacredito_dat
    call DesenhaTela
    mov bx, 1h
  loops1:
    mov ax, 0800h                     ;interrupcao de teclado
    int 21h
    cmp al, tenter
    je teclou_enter
    cmp al, tesc
    je retornaTelaCredito
    jmp loops1
  teclou_enter:
    cmp bx, 1h
    je tela_creditos2
    cmp bx, 2h
    je telacred1
  tela_creditos2:
    mov dx, offset telacredito2_pal
    call CarregaPaleta
    mov dx, offset telacredito2_dat
    call DesenhaTela
    mov bx, 2h
    jmp loops1
  retornaTelaCredito:
    pop ax dx bx
    ret
ProcTelaCredito ENDP
;*******Finaliza Procedimento ProcTelaCredito**************
