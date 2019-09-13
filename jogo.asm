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

.model medium
.386
.stack
.data
include Dados.inc
.code
include Funcoes.inc
include Menu.inc
include Recordes.inc
start:
      mov ax, 0013h ; seta modo video 320*200
      int 10h
      call LeScore  ; carrega o arquivo "scores.dat" na memoria
      call CriaMenu ; inicia o menu principal

      call LimpaTela ; limpa a tela quando sai do jogo, e coloca no modo texto
      mov ah, 4ch    ; volta pro DOS
      int 21h
      
end start
