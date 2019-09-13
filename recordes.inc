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

;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Proc LeScore                                                                 x
;x Armazena na memória os valores do arquvo de score                            x
;x                                                                              x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
LeScore PROC NEAR
pusha
push di es ds
        mov ax, @data
        mov ds, ax
        mov es, ax
mov dx, offset scoreFile
mov di, offset ScName1
mov cx, 60         ;;O tamanho de cada nome*5 + tamanho de cada score*5
call ReadFile
pop ds es di
popa
ret
LeScore ENDP
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Proc EscreveScore                                                            x
;x Escreve no arquivo score.dat os nomes e pontos da memória                    x
;x                                                                              x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
EscreveScore PROC NEAR
pusha
; DS aponta para Segmento de Dados
        mov ax, @data
        mov ds, ax

        ; abre arquivo de Score
        mov dx, offset scoreFile
        mov ax, 3D01h
        int 21h

        ; copia Handle para BX
        mov bx, ax

        ; copia Scores da memoria para Arquivo
        mov dx, offset ScName1
        mov cx, 60 ;;O tamanho de cada nome*5 + tamanho de cada score*5
        mov ah, 40h
        int 21h

        ; fecha arquivo
        mov ah, 3Eh
        int 21h
popa
ret
EscreveScore ENDP

;--------------------------------------------------------------------------------
; Proc CheckHighScore
; Verifica se o jogador obteve um HighScore
;--------------------------------------------------------------------------------

    CheckHighScore   PROC    NEAR
        ; empilha registradores
        pusha
        push ds es

        ; DS e ES apontam para o segmento de dados
        mov ax, @data
        mov ds, ax
        mov es, ax

        ; coloca em AX a pontuacao
        mov ax, [ds:Pontos]

        ; verifica se eh maior que o quinto colocado
        cmp ax, [ds:ScScore5]
        jle CHSExit

        ; verifica se eh maior que o quarto colocado
        cmp ax, [ds:ScScore4]
        jg  CHSPos4

        ; ES:DI aponta para o nome do quinto colocado,
        ; enquanto eh escrito seu valor no score
        mov [ds:ScScore5], ax
        mov colocacao, 5
        mov ScName5[0], " "
        mov ScName5[1], " "
        mov ScName5[2], " "
        mov ScName5[3], " "
        mov ScName5[4], " "
        mov ScName5[5], " "
        mov ScName5[6], " "
        mov ScName5[7], " "
        call PegaNome
        push dx
        mov dl, Array_nome[0]
        mov ScName5[0], dl
        mov dl, Array_nome[1]
        mov ScName5[1], dl
        mov dl, Array_nome[2]
        mov ScName5[2], dl
        mov dl, Array_nome[3]
        mov ScName5[3], dl
        mov dl, Array_nome[4]
        mov ScName5[4], dl
        mov dl, Array_nome[5]
        mov ScName5[5], dl
        mov dl, Array_nome[6]
        mov ScName5[6], dl
        mov dl, Array_nome[7]
        mov ScName5[7], dl
        mov ScName5[8], '$'
        pop dx
        jmp CHSExit

    CHSPos4:
        ; transfere o quarto para o quinto colocado
        mov di, offset ScName5
        mov si, offset ScName4
        mov cx, 12
        rep movsb

        ; verifica se eh maior que o terceiro colocado
        cmp ax, [ds:ScScore3]
        jg  CHSPos3

        ; ES:DI aponta para o nome do quinto colocado,
        ; enquanto eh escrito seu valor no score
        mov [ds:ScScore4], ax
        mov colocacao, 4
        mov ScName4[0], " "
        mov ScName4[1], " "
        mov ScName4[2], " "
        mov ScName4[3], " "
        mov ScName4[4], " "
        mov ScName4[5], " "
        mov ScName4[6], " "
        mov ScName4[7], " "
        call PegaNome
        push dx
        mov dl, Array_nome[0]
        mov ScName4[0], dl
        mov dl, Array_nome[1]
        mov ScName4[1], dl
        mov dl, Array_nome[2]
        mov ScName4[2], dl
        mov dl, Array_nome[3]
        mov ScName4[3], dl
        mov dl, Array_nome[4]
        mov ScName4[4], dl
        mov dl, Array_nome[5]
        mov ScName4[5], dl
        mov dl, Array_nome[6]
        mov ScName4[6], dl
        mov dl, Array_nome[7]
        mov ScName4[7], dl
        mov ScName4[8], '$'
        pop dx
        jmp CHSExit

    CHSPos3:
        ; transfere o quarto para o quinto colocado
        mov di, offset ScName4
        mov si, offset ScName3
        mov cx, 12
        rep movsb

        ; verifica se eh maior que o segundo colocado
        cmp ax, [ds:ScScore2]
        jg  CHSPos2

        ; ES:DI aponta para o nome do quinto colocado,
        ; enquanto eh escrito seu valor no score
        mov [ds:ScScore3], ax
        mov colocacao, 3
        mov ScName3[0], " "
        mov ScName3[1], " "
        mov ScName3[2], " "
        mov ScName3[3], " "
        mov ScName3[4], " "
        mov ScName3[5], " "
        mov ScName3[6], " "
        mov ScName3[7], " "
        call PegaNome
        push dx
        mov dl, Array_nome[0]
        mov ScName3[0], dl
        mov dl, Array_nome[1]
        mov ScName3[1], dl
        mov dl, Array_nome[2]
        mov ScName3[2], dl
        mov dl, Array_nome[3]
        mov ScName3[3], dl
        mov dl, Array_nome[4]
        mov ScName3[4], dl
        mov dl, Array_nome[5]
        mov ScName3[5], dl
        mov dl, Array_nome[6]
        mov ScName3[6], dl
        mov dl, Array_nome[7]
        mov ScName3[7], dl
        mov ScName3[8], '$'
        pop dx
        jmp CHSExit

    CHSPos2:
        ; transfere o quarto para o quinto colocado
        mov di, offset ScName3
        mov si, offset ScName2
        mov cx, 12
        rep movsb

        ; verifica se eh maior que o terceiro colocado
        cmp ax, [ds:ScScore1]
        jg  CHSPos1

        ; ES:DI aponta para o nome do quinto colocado,
        ; enquanto eh escrito seu valor no score
        mov [ds:ScScore2], ax
        mov colocacao, 2
        mov ScName2[0], " "
        mov ScName2[1], " "
        mov ScName2[2], " "
        mov ScName2[3], " "
        mov ScName2[4], " "
        mov ScName2[5], " "
        mov ScName2[6], " "
        mov ScName2[7], " "
        call PegaNome
        push dx
        mov dl, Array_nome[0]
        mov ScName2[0], dl
        mov dl, Array_nome[1]
        mov ScName2[1], dl
        mov dl, Array_nome[2]
        mov ScName2[2], dl
        mov dl, Array_nome[3]
        mov ScName2[3], dl
        mov dl, Array_nome[4]
        mov ScName2[4], dl
        mov dl, Array_nome[5]
        mov ScName2[5], dl
        mov dl, Array_nome[6]
        mov ScName2[6], dl
        mov dl, Array_nome[7]
        mov ScName2[7], dl
        mov ScName2[8], '$'
        pop dx

        jmp CHSExit

    CHSPos1:
        ; transfere o quarto para o quinto colocado
        mov di, offset ScName2
        mov si, offset ScName1
        mov cx, 12
        rep movsb

        ; ES:DI aponta para o nome do quinto colocado,
        ; enquanto eh escrito seu valor no score
        mov [ds:ScScore1], ax
        mov colocacao, 1
        mov ScName1[0], " "
        mov ScName1[1], " "
        mov ScName1[2], " "
        mov ScName1[3], " "
        mov ScName1[4], " "
        mov ScName1[5], " "
        mov ScName1[6], " "
        mov ScName1[7], " "
        call PegaNome
        push dx
        mov dl, Array_nome[0]
        mov ScName1[0], dl
        mov dl, Array_nome[1]
        mov ScName1[1], dl
        mov dl, Array_nome[2]
        mov ScName1[2], dl
        mov dl, Array_nome[3]
        mov ScName1[3], dl
        mov dl, Array_nome[4]
        mov ScName1[4], dl
        mov dl, Array_nome[5]
        mov ScName1[5], dl
        mov dl, Array_nome[6]
        mov ScName1[6], dl
        mov dl, Array_nome[7]
        mov ScName1[7], dl
        mov ScName1[8], '$'
        pop dx

        CHSExit:
        ; desempilha registradores
        pop es ds
        popa
        ret
    CheckHighScore   ENDP


;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
;x Proc PegaNome                                                                x
;x Mostra tela que solicita o jogador que teve score alto a digitar seu nome    x
;x                                                                              x
;xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
PegaNome PROC NEAR
pusha
mov dx, offset telarec_pal
call CarregaPaleta
mov dx, offset telarec_dat
call DesenhaTela

mov podeSair, 0
mov espera, 0

cmp colocacao, 1
jne p2
mov si, offset PerguntaNome1
  mov linhaRec, 46
  push cx dx bx
  ; 1º nome / score
  mov ch, 46
  mov dx, offset ScName
  mov bx, Pontos
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 60
  mov dx, offset ScName2
  mov bx, ScScore2
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 74
  mov dx, offset ScName3
  mov bx, ScScore3
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 88
  mov dx, offset ScName4
  mov bx, ScScore4
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 102
  mov dx, offset ScName5
  mov bx, ScScore5
  call imprimeRecordes

  pop bx dx cx

jmp depois

p2:
cmp colocacao, 2
jne p3
mov si, offset PerguntaNome2
  mov linhaRec, 60
  push cx dx bx
  ; 1º nome / score
  mov ch, 46
  mov dx, offset ScName1
  mov bx, ScScore1
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 60
  mov dx, offset ScName
  mov bx, Pontos
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 74
  mov dx, offset ScName3
  mov bx, ScScore3
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 88
  mov dx, offset ScName4
  mov bx, ScScore4
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 102
  mov dx, offset ScName5
  mov bx, ScScore5
  call imprimeRecordes

  pop bx dx cx
jmp depois

p3:
cmp colocacao, 3
jne p4
mov si, offset PerguntaNome3
  mov linhaRec, 74

  push cx dx bx
  ; 1º nome / score
  mov ch, 46
  mov dx, offset ScName1
  mov bx, ScScore1
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 60
  mov dx, offset ScName2
  mov bx, ScScore2
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 74
  mov dx, offset ScName
  mov bx, Pontos
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 88
  mov dx, offset ScName4
  mov bx, ScScore4
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 102
  mov dx, offset ScName5
  mov bx, ScScore5
  call imprimeRecordes

  pop bx dx cx
jmp depois

p4:
cmp colocacao, 4
jne p5
mov si, offset PerguntaNome4

  mov linhaRec, 88
  push cx dx bx
  ; 1º nome / score
  mov ch, 46
  mov dx, offset ScName1
  mov bx, ScScore1
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 60
  mov dx, offset ScName2
  mov bx, ScScore2
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 74
  mov dx, offset ScName3
  mov bx, ScScore3
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 88
  mov dx, offset ScName
  mov bx, Pontos
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 102
  mov dx, offset ScName5
  mov bx, ScScore5
  call imprimeRecordes

  pop bx dx cx
jmp depois
p5:
cmp colocacao, 5
jne fimLoop
mov si, offset PerguntaNome5

  mov linhaRec, 102
  push cx dx bx
  ; 1º nome / score
  mov ch, 46
  mov dx, offset ScName1
  mov bx, ScScore1
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 60
  mov dx, offset ScName2
  mov bx, ScScore2
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 74
  mov dx, offset ScName3
  mov bx, ScScore3
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 88
  mov dx, offset ScName4
  mov bx, ScScore4
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 102
  mov dx, offset ScName
  mov bx, Pontos
  call imprimeRecordes

  pop bx dx cx
jmp depois

depois:

mov di, 50
mov cl, 15
mov dh, 0ffh
call DesenharFrase


mov bx, 8
mov di, 89
mov Array_nome[0], ' '
mov Array_nome[1], ' '
mov Array_nome[2], ' '
mov Array_nome[3], ' '
mov Array_nome[4], ' '
mov Array_nome[5], ' '
mov Array_nome[6], ' '
mov Array_nome[7], ' '
mov Array_nome[8], '$'

PegaLetra:
cmp bx, 0
je Fim1

mov ax , 0000h
int 16h
cmp al, tenter
je FimLoop

cmp al, 08h
je TrataBack

cmp al, espaco
je TrataEspaco

cmp al, 00h ;tecla especial
je PegaLetra

;*******************************************
;CODIGO PARA COLOCAR OS NOMES NO LUGAR CERTO
;*******************************************


add di, 7
mov cl, linhaRec
mov dh, 000h



call DesenharLetra
push dx
mov dx, 8
sub dx,bx
push bx
mov bx,dx

mov Array_nome[bx],al
pop bx dx
dec bx

jmp PegaLetra

TrataEspaco:
add di, 7
mov cl, linhaRec
mov dh, 0ffh
call DesenharLetra
push dx
mov dx, 8
sub dx,bx
push bx
mov bx,dx

mov Array_nome[bx]," "
pop bx dx
dec bx
jmp PegaLetra

TrataBack:
;sub di, 7
push dx
mov dx, 8
sub dx,bx
push bx
mov bx,dx

mov Array_nome[bx]," "
pop bx dx

cmp bx,8
je PegaLetra
mov dh, 0ffh
mov al, 20h
inc bx
call DesenharLetra
mov dh, 000h
sub di, 7
jmp PegaLetra

Fim1:
mov ax , 0000h
int 16h
cmp al, 08h
je TrataBack
cmp al, tenter
jne Fim1

FimLoop:
popa
ret
PegaNome ENDP


;**********************************************
; Procedimento que imprime o valor dos Recordes na tela
; ch -> linha
; dx -> offset do nome
; bx -> numero
;**********************************************

imprimeRecordes PROC NEAR

pusha
push di si

  mov ax, @data
  mov ds, ax

  mov si, dx
  mov dh, 00h
  mov di, 96
  mov cl, ch

  push cx bx
  call DesenharFrase
  pop bx cx
  mov dl, ch
  mov ah, ch
  push ax
  mov ax, bx
  call ConverterDecimal
  pop ax
  mov dh, 00h
  mov di, 200
  ;mov ah, ch;ch

  call DesenharNumero


pop si di
popa
ret
imprimeRecordes ENDP

;**********************************************
;Imprime todos recordes na tela
;**********************************************
imprimeTodos PROC NEAR

  mov podeSair, 0
  mov espera, 3
  push cx dx bx
  ; 1º nome / score
  mov ch, 49
  mov dx, offset ScName1
  mov bx, ScScore1
  call imprimeRecordes

  ; 2º nome / score
  mov ch, 63
  mov dx, offset ScName2
  mov bx, ScScore2
  call imprimeRecordes

  ; 3º nome / score
  mov ch, 77
  mov dx, offset ScName3
  mov bx, ScScore3
  call imprimeRecordes

  ; 4º nome / score
  mov ch, 91
  mov dx, offset ScName4
  mov bx, ScScore4
  call imprimeRecordes

  ; 5º nome / score
  mov ch, 105
  mov dx, offset ScName5
  mov bx, ScScore5
  call imprimeRecordes

  pop bx dx cx
  ret

imprimeTodos ENDP
