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

;******************ARQUIVOS DE DADOS

     ;figuras contidas em arquivos
     menuIniciar_dat       db 'figuras/menuInic.dat',0
     menuRecordes_dat      db 'figuras/menuHigh.dat',0
     menuCreditos_dat      db 'figuras/menuCred.dat',0
     menuAjuda_dat         db 'figuras/menuHelp.dat',0
     menuSair_dat          db 'figuras/menuSair.dat',0
     menuIniciar_pal       db 'figuras/menuInic.pal',0
     menuRecordes_pal      db 'figuras/menuHigh.pal',0
     menuCreditos_pal      db 'figuras/menuCred.pal',0
     menuAjuda_pal         db 'figuras/menuHelp.pal',0
     menuSair_pal          db 'figuras/menuSair.pal',0
     fundo_dat             db 'figuras/fundo.dat',0
     fundo_pal             db 'figuras/fundo.pal',0
     telahist1_dat         db 'figuras/hist1.dat',0
     telahist1_pal         db 'figuras/hist1.pal',0
     telahist2_dat         db 'figuras/hist2.dat',0
     telahist2_pal         db 'figuras/hist2.pal',0
     telahist3_dat         db 'figuras/hist3.dat',0
     telahist3_pal         db 'figuras/hist3.pal',0
     telahist4_dat         db 'figuras/hist4.dat',0
     telahist4_pal         db 'figuras/hist4.pal',0
     telacredito_dat       db 'figuras/credito.dat',0
     telacredito_pal       db 'figuras/credito.pal',0
     telacredito2_dat      db 'figuras/credito2.dat',0
     telacredito2_pal      db 'figuras/credito2.pal',0
     telaajuda_dat         db 'figuras/ajuda.dat',0
     telaajuda_pal         db 'figuras/ajuda.pal',0
     telasair_dat          db 'figuras/telasair.dat',0
     telasair_pal          db 'figuras/telasair.pal',0
     telasaindo_dat        db 'figuras/tsaindo.dat',0
     telasaindo_pal        db 'figuras/tsaindo.pal',0
     telasaiu_dat          db 'figuras/telasaiu.dat',0
     telasaiu_pal          db 'figuras/telasaiu.pal',0
     telarecordes_dat      db 'figuras/recordes.dat',0
     telarecordes_pal      db 'figuras/recordes.pal',0
     telarec_dat           db 'figuras/rec.dat',0
     telarec_pal           db 'figuras/rec.pal',0
     borda_dat             db 'figuras/borda.dat',0
     borda_pal             db 'figuras/borda.pal',0
     fases_dat             db 'figuras/fases.dat',0
     fases_pal             db 'figuras/fases.pal',0
     over_dat             db 'figuras/over.dat',0
     over_pal             db 'figuras/over.pal',0
     zerou_dat             db 'figuras/ganhou.dat',0
     zerou_pal             db 'figuras/ganhou.pal',0



     ; buffer das paletas
     palleteBuffer         DB  768   dup(?)
     ;mensagens de erro
     msg1                  DB "ERRO NO ARQUIVO$"
     msg2                  DB "ERRO NO ARQUIVO$"
     msg3                  DB "ERRO NO ARQUIVO$"
     msg4                  DB "ERRO NO ARQUIVO$"
     msgSair               DB "DESEJA SAIR REALMENTE ? (S/N)",0
     fase1                 DB "FASE 1",0
     fase2                 DB "FASE 2",0
     fase3                 DB "FASE 3",0
     
     teclas                db "PRESSIONE QUALQUER TECLA PARA CONTINUAR",0

     scoreFile             db      'score.dat', 0
     ;posição de sergio na tela
     posicao_tela          dw 56780;57740       ;calcular o meio MESMO!!
     ;posição do tiro na tela
    
     ;booleano que verifica se o tiro da sergio esta na tela
     tiro_na_tela          db 0
     espera                db ?
     ;****Variaveis para tratar o teclado
     PressionouTecla       db 0
     varLeft               db 0
     varRight              db 0
     varEspaco             db 0
     varEsc                db 0
     varPause              db 0
     varS                  db 0
     varN                  db 0

     podeSair              db 0
     sair_hist             db 0


     OldInt15_SEG            DW      ? ;guardará a interrupção do teclado
     OldInt15_OFFSET         DW      ? ;

     cont                    db 30
     boolN                   db 0       ; diz se tah dormindo ou acordado
     posicaoTampa            dw 10332
     tampa_na_tela           db 0
     coluna                  dw ?
     posicaoInimigo          dw 7132

     array_inimigo           dw 0, 30, 60, 90, 120
                             dw 6080, 6110, 6140, 6170, 6200
                             dw 12160, 12190, 12220, 12250, 12280
                             dw 18240, 18270, 18300, 18330, 18360

     inimigos_vivos          db 20

     direcaoInimigos         dw 1 ; 0 eh esquerda e 1 eh direita
     coluna1                 db 4
     coluna2                 db 4
     coluna3                 db 4
     coluna4                 db 4
     coluna5                 db 4
     var_esquerdo            dw 0            ;
     var_direito             dw 133          ; tamanho total da largura da fileira dos inimigos
     linha                   db 22           ;var q guarda a linha do inimigo no momento
     tirosInimigos           dw 12d dup(0)
     balas_na_tela           dw 0
     Factor                  DW   22121
     Seed                    dw 0
     lim1                    dw 20
     lim2                    dw 25
     nivel                   db 1
     strpause                db "PAUSE",0
     strScore                db "SCORE:",0
     strVidas                db "VIDAS:",0
     Vidas                   db ?
     GameOver                db 0
     SergioMorreu            db 0
     mano_na_tela            db 0           ; var q diz quando emanoel tah na tela
     mano_indo_volta         db 0           ; 0 eh direita ->, 1<-
     posicao_mano            dw 0
     limite_mano             dw 0

;;;*********************variaveis temporarias
     tempScore               dw ?
     tempTempo               dw ?
     tempVida                db ?
     totaltempo              dw 0
     totalscore              dw 0
     totalvidas              dw 600


     mudouFase               db 0

;;;*********************Frases

     penalidade              db "PENALIDADE : ",0
     pontosVidas             db "VIDAS      : ",0
     scoreTotal              db "SCORE TOTAL: ",0

;;;**************SCORE!!!!!!!!!!!!!!!*****************
    Pontos              DW       0
    ; pontuacoes
    ScName              db      "        $",0
    ScName1             db      "Paulo   $",0
    ScScore1            dw      20
    ScName2             db      "Lira    $",0
    ScScore2            dw      15
    ScName3             db      "Emanoel $",0
    ScScore3            dw      10
    ScName4             db      "Carina  $",0
    ScScore4            dw      5
    ScName5             db      "Ana     $",0
    ScScore5            dw      0

    ; conversao de int/ascii
    ScNumber1           db      '00000$'
    ScNumber2           db      '00000$'
    ScNumber3           db      '00000$'
    ScNumber4           db      '00000$'
    ScNumber5           db      '00000$'

    PerguntaNome1       db      "      VOCE BATEU UM RECORDE!!!!$"
                        db      "DIGITE SEU NOME NA PRIMEIRA COLOCAçãO$",0
    PerguntaNome2       db      "      VOCE BATEU UM RECORDE!!!!$"
                        db      "DIGITE SEU NOME NA SEGUNDA COLOCAçãO$",0
    PerguntaNome3       db      "      VOCE BATEU UM RECORDE!!!!$"
                        db      "DIGITE SEU NOME NA TERCEIRA COLOCAçãO$",0
    PerguntaNome4       db      "      VOCE BATEU UM RECORDE!!!!$"
                        db      "DIGITE SEU NOME NA QUARTA COLOCAçãO$",0
    PerguntaNome5       db      "      VOCE BATEU UM RECORDE!!!!$"
                        db      "DIGITE SEU NOME NA QUINTA COLOCAçãO$",0

    Array_nome          db      9 dup(?)
    colocacao           db      ?        ; GUARDA A COLOCAÇÃO SE HOUVE QUEBRA DE RECORDE
    linhaRec            db      ?        ;LInha da na tela de recorde onde será escrito o nome

    linhaPulo           db      4
    tamPulo             dw      1280

;*************************Tempo
tempoRTC    dw 0
timeCounter   dw 0
contadorRTC    db -1
;******************************
;***********************SOM
     habilitaSom        db      1
;************************
;******** CONTANTES
     up                      EQU     48h
     down                    EQU     50h
     dir                     EQU     50h
     tesc                    EQU     1Bh
     tenter                  EQU     0Dh
     esquerda                EQU     4Bh
     direita                 EQU     4Dh
     espaco                  EQU     20h



;******** CARACTERES

Letras            	DB      38h,44h,44h,7ch,44h,44h,44h,00h,00h,00h ;A
	            	DB      78h,44h,44h,78h,44h,44h,78h,00h,00h,00h ;B
	                DB      38h,44h,40h,40h,40h,44h,38h,00h,00h,00h ;C
	                DB      78h,44h,44h,44h,44h,44h,78h,00h,00h,00h ;D
	                DB      7ch,40h,40h,78h,40h,40h,7ch,00h,00h,00h ;E
	                DB      7ch,40h,40h,78h,40h,40h,40h,00h,00h,00h ;F
	                DB      38h,44h,40h,5ch,44h,44h,38h,00h,00h,00h ;G
	                DB      44h,44h,44h,7ch,44h,44h,44h,00h,00h,00h ;H
	                DB      7ch,10h,10h,10h,10h,10h,7ch,00h,00h,00h ;I
	                DB      04h,04h,04h,04h,04h,44h,38h,00h,00h,00h ;J
	                DB      44h,48h,50h,60h,50h,48h,44h,00h,00h,00h ;K
	                DB      40h,40h,40h,40h,40h,40h,7ch,00h,00h,00h ;L
	                DB      44h,6ch,54h,44h,44h,44h,44h,00h,00h,00h ;M
	                DB      44h,64h,54h,4ch,44h,44h,44h,00h,00h,00h ;N
	                DB      38h,44h,44h,44h,44h,44h,38h,00h,00h,00h ;O
	                DB      78h,44h,44h,78h,40h,40h,40h,00h,00h,00h ;P
	                DB      38h,44h,44h,44h,44h,4ch,3ch,00h,00h,00h ;Q
	                DB      78h,44h,44h,78h,44h,44h,44h,00h,00h,00h ;R
	                DB      38h,44h,40h,38h,04h,44h,38h,00h,00h,00h ;S
	                DB      7ch,10h,10h,10h,10h,10h,10h,00h,00h,00h ;T
	                DB      44h,44h,44h,44h,44h,44h,38h,00h,00h,00h ;U
	                DB      44h,44h,44h,44h,44h,28h,10h,00h,00h,00h ;V
	                DB      44h,44h,44h,44h,54h,6ch,44h,00h,00h,00h ;W
	                DB      44h,44h,28h,10h,28h,44h,44h,00h,00h,00h ;X
	                DB      44h,44h,28h,10h,10h,10h,10h,00h,00h,00h ;Y
	                DB      7ch,04h,08h,10h,20h,40h,7ch,00h,00h,00h ;Z
    	                DB      00h,00h,38h,04h,3ch,44h,3ch,00h,00h,00h ;a
	            	DB      40h,40h,78h,44h,44h,44h,78h,00h,00h,00h ;b
	            	DB    	00h,00h,3ch,40h,40h,40h,3ch,00h,00h,00h ;c
	            	DB      04h,04h,3ch,44h,44h,44h,3ch,00h,00h,00h ;d
	            	DB  	00h,00h,38h,44h,7ch,40h,3ch,00h,00h,00h ;e
	            	DB      38h,44h,40h,40h,78h,40h,40h,00h,00h,00h ;f
	            	DB  	00h,00h,38h,44h,44h,44h,38h,04h,44h,38h ;g
	            	DB      40h,40h,78h,44h,44h,44h,44h,00h,00h,00h ;h
	            	DB      08h,00h,00h,18h,08h,08h,3ch,00h,00h,00h ;i
		        DB      00h,08h,00h,18h,08h,08h,08h,08h,48h,30h ;j
		        DB      40h,40h,48h,50h,60h,50h,48h,00h,00h,00h ;k
		        DB      18h,08h,08h,08h,08h,08h,3ch,00h,00h,00h ;l
                        DB      00h,00h,68h,54h,54h,54h,54h,00h,00h,00h ;m
	            	DB      00h,00h,78h,44h,44h,44h,44h,00h,00h,00h ;n
	            	DB      00h,00h,38h,44h,44h,44h,38h,00h,00h,00h ;o
	            	DB      00h,00h,78h,44h,44h,44h,78h,40h,40h,40h ;p
	            	DB      00h,00h,38h,44h,44h,44h,3ch,04h,04h,04h ;q
	            	DB      00h,00h,5ch,60h,40h,40h,40h,00h,00h,00h ;r
	            	DB      00h,00h,3ch,40h,38h,04h,78h,00h,00h,00h ;s
	            	DB      20h,78h,20h,20h,20h,24h,18h,00h,00h,00h	;t
	            	DB      00h,00h,44h,44h,44h,44h,3ch,00h,00h,00h ;u
	            	DB      00h,00h,44h,44h,44h,28h,10h,00h,00h,00h ;v
	            	DB      00h,00h,54h,54h,54h,54h,28h,00h,00h,00h ;w
	            	DB      00h,00h,44h,28h,10h,28h,44h,00h,00h,00h ;x
	            	DB      00h,00h,44h,44h,44h,44h,3ch,04h,04h,38h ;y
	            	DB      00h,00h,7ch,08h,10h,20h,7ch,00h,00h,00h ;z
	            	DB      00h,00h,00h,00h,00h,60h,20h,40h,00h,00h ;,(VIRGULA)
	            	DB      78h,84h,08h,10h,20h,00h,20h,00h,00h,00h  ; ?
	            	DB      00h,00h,00h,18h,00h,18h,00h,00h,00h,00h  ; :
	            	DB      01h,01h,01h,01h,01h,01h,00h,01h,00h,00h   ; !
	            	DB      00h,00h,00h,0fh,00h,00h,00h,00h,00h,00h   ; -
	            	DB      00h,00h,00h,00h,00h,18h,18h,00h,00h,00h ; .
	            	DB      02h,04h,38h,04h,3ch,44h,3ch,00h,00h,00h ;á
	            	DB      24h,58h,38h,04h,3ch,44h,3ch,00h,00h,00h ;ã
	            	DB      10h,28h,38h,04h,3ch,44h,3ch,00h,00h,00h ;â
	            	DB      04h,08h,00h,18h,08h,08h,3ch,00h,00h,00h  ;í
	            	DB      00h,00h,3ch,40h,40h,40h,3ch,01h,08h,00h; ç
	            	DB      02h,04h,38h,44h,7ch,40h,3ch,00h,00h,00h ;é
	            	DB      10h,28h,38h,44h,7ch,40h,3ch,00h,00h,00h ;ê
	            	DB      0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh ;espaço
	            	DB      38h,44h,4ch,54h,64h,44h,38h,000h,000h,000h ;0
	            	DB      10h,30h,10h,10h,10h,10h,38h,000h,000h,000h ;1
                        DB      38h,44h,04h,18h,20h,40h,7ch,00h,00h,00h ;2
                        DB      38h,44h,04h,18h,04h,44h,38h,00h,00h,00h ;3
                        DB      44h,44h,44h,7ch,04h,04h,04h,00h,00h,00h ;4
                        DB      7ch,40h,40h,78h,04h,44h,38h,00h,00h,00h ;5
                        DB      38h,44h,40h,78h,44h,44h,38h,00h,00h,00h ;6
                        DB      7ch,04h,08h,10h,10h,10h,10h,00h,00h,00h ;7
                        DB      38h,44h,44h,38h,44h,44h,38h,00h,00h,00h ;8
                        DB      38h,44h,44h,3ch,04h,44h,38h,00h,00h,00h ;9
                        DB      0eh,18h,30h,30h,30h,18h,0eh,00h,00h,00h ;(
                        DB      70h,18h,0ch,0ch,0ch,18h,70h,00h,00h,00h ;)
                        DB      03h,06h,0ch,18h,30h,60h,0c0h,00h,00h,00h ;/

Numeros                 DB      38h,44h,4ch,54h,64h,44h,38h ;0
                        DB      10h,30h,10h,10h,10h,10h,38h ;1
                        DB      38h,44h,04h,18h,20h,40h,7ch ;2
                        DB      38h,44h,04h,18h,04h,44h,38h ;3
                        DB      44h,44h,44h,7ch,04h,04h,04h ;4
                        DB      7ch,40h,40h,78h,04h,44h,38h ;5
                        DB      38h,44h,40h,78h,44h,44h,38h ;6
                        DB      7ch,04h,08h,10h,10h,10h,10h ;7
                        DB      38h,44h,44h,38h,44h,44h,38h ;8
                        DB      38h,44h,44h,3ch,04h,44h,38h ;9

;******** FIGURAS

historinha1   db "  Todas as tardes no Reino da Sleeper Lândia,$"
              db "o Grandioso Imperador Sérgio Cavalcante lutava$"
              db "contra seus mais terríveis inimigos para$"
              db "fazê-los despertar. O Imperador queria$"
              db "acabar logo com essas batalhas para ir$"
              db "visitar o Encantado Reino de Tamandaré.$"
              db "  No entanto, seus inimigos teimavam em$"
              db "prolongar o tempo de guerra, fazendo$"
              db "uso de uma poderosa arma: o sono.  *$",0
historinha2   db "  Essa arma colocaria os planos do Imperador$"
              db "por água abaixo, levando a guerra para o$"
              db "chamado estágio da Recuperação Final.$"
              db "Mas, o grande imperador tinha vários$"
              db "amigos no Reino da Pilot. Eles enviaram$"
              db "ao Grande Sérgio uma arma poderosíssima$"
              db "e muito nova no mercado:$",0

historinha3   db "  Desde então, o poderoso Sérgio$"
              db "vem travando duras batalhas com$"
              db "seus inimigos.$"
              db "$"
              db "  O que você acha de ajudá-lo?",0
              
              

fase1_2       db "     Essa foi muito fácil!$"
              db "$"
              db "  Mas, depois de uma aula de$"
              db "  Metodologia terei que ser$"
              db "   muito bom na pontaria.$",0

fase2_3       db "   Estou ficando bom nisso.$"
              db "$"
              db "Vamos ver quanto tempo eu levarei$"
              db"   para acordá-los depois deles$"
              db "terem assistido a uma aula de$"
              db "      Carlos Ferraz...$",0

gameover_hist db "Desse jeito a turma vai acabar indo$"
              db "pra Final, e Sergio,pobrezinho, vai$"
              db "      ficar sem praia.$"
              db "$"
              db "                     Tente novamente!$",0


zerou_hist    db "                    Ufa.$"
              db "$"
              db "Depois de momentos tão difíceis durante a batalha,$"
              db "eu consegui vencer, acordando todos os alunos$"
              db "que teimavam em ficar o tempo todo cochilando$"
              db "em minhas as aulas.$"
              db "Agora, estou alegre porque finalmente fiquei$"
              db "livre do estágio da Recuperação Final, e poderei$"
              db "passar vários dias me divertindo no encantado$"
              db "            Reino de Tamandaré.$",0







  CrA            EQU 01h ;VERMELHO
  CrB            EQU 02h ;VERDE
  CrC            EQU 03h ;AMARELO FEIO
  CrD            EQU 04h ;AZUL ESCURO
  CrE            EQU 05h ;LILAIS
  CrF            EQU 06h ;AZUL MEIO ESTRANHO
  CrG            EQU 07h ;CINZA QUASE BRANCO
  CrH            EQU 08h ;CORES ESTRANHAS QUASE BRANCO
  CrI            EQU 09h ;CORES ESTRANHAS QUASE BRANCO
  CrJ            EQU 0Ah ;CORES ESCURAS
  CrL            EQU 0Bh ;CORES ESCURAS
  CrK            EQU 0Ch
  CrM            EQU 0Dh ;LARANJA
  CrN            EQU 0Eh ;LARANJA INTERMEDIARIO
  CrO            EQU 0Fh ;LARANJA CLARO
  crp            EQU 2Fh ;OUTRO AMARELO
  CrQ            EQU 2Eh ;AMAERELO MAIS CLARO
  CrR            EQU 2Ch ;VERDE CLARA (PUXANDO PARA O AMARELO)
  CrS            EQU 2Dh ;AMARELO
  CrT            EQU 2Ah ;VERDE
  CrU            EQU 2Bh ;VERDE MAIS CLARO
  CrV            EQU 20h ;VERDE
  CrX            EQU 21h ;VERDE
  BCO            EQU 0FFh;BRANCO
  PTO            EQU 00h ;PRETO
                     ;27h LARANJA MOSTARDA


ProfessorTampa   DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,BCO,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07H,BCO,BCO,BCO,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07H,07h,BCO,BCO,BCO,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,PTO,PTO,PTO,PTO,PTO,PTO,CrM,CrM,CrM,07h,07h,07h,07H,BCO,BCO,BCO,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,PTO,PTO,PTO,PTO,07h,BCO,BCO,PTO,CrM,CrM,07h,07h,07h,07H,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,BCO,PTO,PTO,07H,07h,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,PTO,07H,07h,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,CrN,CrN,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07H,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,07H,07H,PTO,PTO,PTO,PTO,07H,07H,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h


ProfessorSemTampa DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07H,07H,07H,07H,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07h,07h,CrM,CrM,CrM,CrM,07h,07h,07H,07h,07H,07H,07H,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,CrM,PTO,PTO,PTO,PTO,PTO,PTO,CrM,CrM,CrM,07h,07h,07h,07H,07H,07H,07H,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,CrM,CrM,PTO,PTO,PTO,PTO,07h,BCO,BCO,PTO,CrM,CrM,07h,07h,07h,07H,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,BCO,PTO,PTO,07H,07h,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,PTO,07H,07h,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,CrD,CrD,CrD,07h,07h,07H
                 DB 07h,07h,CrN,CrN,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07H,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,CrN,CrN,CrN,BCO,BCO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,BCO,BCO,CrN,CrN,CrN,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07H,07H,07H,07H,07H,PTO,PTO,PTO,PTO,07H,07H,07H,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h





Tampa            DB 07h,BCO,07h
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB 07h,07h,07h
                 DB 07h,07h,07h

limpaTampa       DB 07h,07h,07h
                 DB 07h,07h,07h
                 DB 07h,07h,07h
                 DB 07h,07h,07h
                 DB 07h,07h,07h
                 DB 07h,07h,07h


inimigo          DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,PTO,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,PTO,07h,07h,07h,07h,PTO,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,PTO,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,CrA,CrA,CrA,CrA,CrA,CrA,CrA,07h,07h,07h
                 DB 07h,07h,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,07h,07h
                 DB 07h,07h,CrA,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrA,07h,07h
                 DB 07h,07h,CrA,6fh,PTO,PTO,6fh,PTO,PTO,6fh,CrA,07h,07h
                 DB 07h,07h,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,07h,07h
                 DB 07h,07h,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,07h,07h
                 DB 07h,07h,6fh,6fh,6fh,CrM,CrM,CrM,6fh,6fh,6fh,07h,07h
                 DB 07h,07h,07h,6fh,6fh,6fh,6fh,6fh,6fh,6fh,07h,07h,07h



inimigo2         DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,CrA,CrA,CrA,CrA,CrA,CrA,CrA,07H,07H,07H
                 DB 07h,07h,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,07h,07h
                 DB 07h,07h,CrA,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrA,07h,07h
                 DB 07h,07h,CrA,6fh,PTO,6fh,6fh,6fh,PTO,6fh,CrA,07h,07h
                 DB 07h,07h,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,6fh,07h,07h
                 DB 07h,07h,6fh,6fh,CrM,6fh,6fh,6fh,CrM,6fh,6fh,07h,07h
                 DB 07h,07h,6fh,6fh,6fh,CrM,CrM,CrM,6fh,6fh,6fh,07h,07h
                 DB 07h,07h,07h,6fh,6fh,6fh,6fh,6fh,6fh,6fh,07h,07h,07h

inimigaA1        DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,PTO,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,PTO,07h,07h,07h,07h,PTO,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,PTO,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h,07h
                 DB 07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,6fh,6fh,6fh,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,PTO,PTO,6fh,PTO,PTO,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,6fh,6fh,6fh,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,6fh,6fh,6fh,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,CrM,CrM,CrM,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,PTO,07H,6fh,6fh,6fh,07H,PTO,PTO,07h,07h

inimigaA2        DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h,07h
                 DB 07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,6fh,6fh,6fh,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,PTO,6fh,6fh,6fh,PTO,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,6fh,6fh,6fh,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,CrM,6fh,6fh,6fh,CrM,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,6fh,6fh,CrM,CrM,CrM,6fh,6fh,PTO,07h,07h
                 DB 07h,07h,PTO,PTO,07H,6fh,6fh,6fh,07H,PTO,PTO,07h,07h

inimigaC1        DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,PTO,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,PTO,07h,07h,07h,07h,PTO,07h,07h
                 DB 07h,07h,07h,07h,PTO,PTO,PTO,07h,07h,PTO,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,CrM,CrM,CrM,CrM,CrM,CrM,CrM,07h,07h,07h
                 DB 07h,07h,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,PTO,PTO,6fh,PTO,PTO,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,CrM,CrM,CrM,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,CrM,07H,6fh,6fh,6fh,07H,CrM,CrM,07h,07h

inimigaC2        DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,CrM,CrM,CrM,CrM,CrM,CrM,CrM,07h,07h,07h
                 DB 07h,07h,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,PTO,6fh,6fh,6fh,PTO,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,6fh,6fh,6fh,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,CrM,6fh,6fh,6fh,CrM,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,6fh,6fh,CrM,CrM,CrM,6fh,6fh,CrM,07h,07h
                 DB 07h,07h,CrM,CrM,07H,6fh,6fh,6fh,07H,CrM,CrM,07h,07h


limpaInimigo     DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h

tiroInimigo      DB 07h,07h,07h
                 DB 07h,07h,07h
                 DB PTO,PTO,PTO
                 DB 07H,07H,PTO
                 DB 07H,PTO,07H
                 DB PTO,PTO,PTO

limpaTiroInimigo  DB 07h,07h,07h
                  DB 07h,07h,07h
                  DB 07h,07h,07h
                  DB 07h,07h,07h
                  DB 07h,07h,07h
                  DB 07h,07h,07h

; FIGURA QUE SERVE PRA APAGAR O QUADRO DE SCORES (39 X 13)
Branco            db BCO, CRD,  CRD, CRD, CRD,  CRD,  CRD,  CRD,  CRD,  CRD,  CRD, CRD,  CRD, CRD,  CRD,  CRD,  CRD, CRD,  CRD, CRD,  CRD,  CRD, CRD,  CRD,  CRD,  CRD,  CRD, CRD,  CRD,  CRD,  CRD,  CRD,  CRD,  CRD, CRD,   CRD, CRD, CRD, BCO
                  db CRD, 0EAh, 0EAh, 0EAh,0EAh, 0EAh, 0EAH, 0EAh, 0EAh, 0EAh, 0EAh,0EAH, 0EAh,0EAh, 0EAh, 0EAh, 0EAH,0EAh, 0EAh,0EAh, 0EAh, 0EAH,0EAh, 0EAh, 0EAh, 0EAh, 0EAH,0EAh, 0EAh, 0EAh, 0EAh, 0EAH, 0EAh, 0EAh, 0EAh, 0eah, 0eah, 0eah, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, BCO, 0EAh, CRD
                  db CRD, 0EAh, 0EAh, 0EAh,0EAh, 0EAh, 0EAH, 0EAh, 0EAh, 0EAh, 0EAh,0EAH, 0EAh,0EAh, 0EAh, 0EAh, 0EAH,0EAh, 0EAh,0EAh, 0EAh, 0EAH,0EAh, 0EAh, 0EAh, 0EAh, 0EAH,0EAh, 0EAh, 0EAh, 0EAh, 0EAH, 0EAh, 0EAh, 0EAh, 0eah, 0eah, 0eah, CRD
                  db BCO, CRD,  CRD, CRD, CRD,  CRD,  CRD,  CRD,  CRD,  CRD,  CRD, CRD,  CRD, CRD,  CRD,  CRD,  CRD, CRD,  CRD, CRD,  CRD,  CRD, CRD,  CRD,  CRD,  CRD,  CRD, CRD,  CRD,  CRD,  CRD,  CRD,  CRD,  CRD, CRD,   CRD, CRD, CRD, BCO


cinza             db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                  db 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h, 07h
                     ; 35, 9

BiroDefault     DB 07h,07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,PTO,07h,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,PTO,07H,07h,07h
                DB 07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,PTO,PTO,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,PTO,PTO,PTO,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h
                 ; 44x11

Biro1           DB 07h,07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,PTO,07h,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,PTO,07H,07h,07h
                DB 07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,PTO,PTO,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,PTO,PTO,PTO,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h
                 ; 44x11

Biro2           DB 07h,07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,PTO,07h,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,PTO,07H,07h,07h
                DB 07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,PTO,PTO,PTO,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,PTO,PTO,PTO,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,CrA,CrA,CrA,CrA,CrA,CrA,Cra,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,PTO,PTO,PTO,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,CrA,PTO,07H,07h,07h
                DB 07h,07h,07h,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,PTO,07H,07h,07h

EmanoelEsq       DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,PTO,07h,07h,07h,07h,07h,07H,07H,07h,07h,07h,07H,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,CrA,CrA,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,CrM,CrM,CrM,CrM,PTO,07h,07h,07h,07H,07H,07h,07h,07h,07H,07H,07h,07h
                 DB 07h,07h,07h,07h,07h,CrA,CrA,CrA,CrA,07h,07h,PTO,PTO,PTO,CrM,37h,37h,37h,37h,37h,CrM,PTO,07h,07h,07h,07H,07H,07h,07h,07h,07H,07H,07h,07h
                 DB 6fh,04h,04h,04h,04h,6fh,6fh,6fh,6fh,PTO,PTO,CrM,CrM,CrM,CrM,CrM,CrM,CrM,37h,CrM,CrM,CrM,PTO,07h,07h,07h,07H,07H,07h,07h,07h,07H,07h,07h
                 DB 6fh,04h,04h,04h,04h,6fh,PTO,6fh,6fh,PTO,CrM,CrM,CrM,CrM,CrM,CrM,CrM,37h,CrM,CrM,CrM,CrM,PTO,04h,04h,04h,04h,04h,04h,04h,PTO,07H,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,6fh,6fh,6fh,PTO,PTO,PTO,PTO,CrM,CrM,CrM,37h,37h,37h,37h,37h,CrM,PTO,04h,04h,04h,04h,04h,04h,04h,PTO,PTO,07h,07h
                 DB 07h,6fh,07h,07h,07h,07h,07h,07h,07h,07h,04h,04H,04h,PTO,PTO,PTO,CrM,CrM,CrM,CrM,CrM,CrM,CrM,PTO,07h,07h,07h,07h,07h,07h,07h,PTO,07h,07h
                 DB 6fh,6fh,04h,04h,04h,04h,04h,04h,04h,04h,04h,04H,04h,04h,04h,04h,PTO,PTO,PTO,CrM,CrM,CrM,CrM,PTO,04h,04h,04h,04h,04h,04h,PTO,07h,07h,07h
                 DB 6fh,6fh,04h,04h,04h,04h,04h,04h,04h,04h,04h,04H,04h,04h,04h,04h,04h,04h,04h,PTO,PTO,PTO,04h,PTO,04h,04h,04h,04h,04h,04h,PTO,PTO,07h,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07h,07h,07h,07h,PTO,PTO,07h,07h,07h,07h,07h,07h,07h,PTO,07h,07h
                 ; 34x11

EmanoelDir       DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,PTO,PTO,PTO,07h,07h,07h,07h,07h,07H,07h,07h,07h,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,CrM,CrM,CrM,CrM,PTO,PTO,PTO,07h,07h,07h,07h,07h,07h,CrA,CrA,07H,07h,07h,07h,07H,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,PTO,37h,37h,37h,37h,37h,CrM,CrM,PTO,PTO,PTO,07h,07h,CrA,CrA,CrA,CrA,07h,07h,07h,07H,07h
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,PTO,CrM,CrM,CrM,CrM,37h,CrM,CrM,CrM,CrM,CrM,CrM,PTO,PTO,6fh,6fh,6fh,6fh,04h,04h,04h,04h,6fH
                 DB 07h,07h,07h,PTO,04h,04H,04h,04h,04h,04h,04H,PTO,CrM,CrM,CrM,37h,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,PTO,6fh,6fh,PTO,6fh,04h,04h,04h,04h,6fH
                 DB 07h,07h,PTO,PTO,04h,04H,04h,04h,04h,04h,04H,PTO,CrM,CrM,37h,37h,37h,37h,37h,CrM,PTO,PTO,PTO,PTO,PTO,6fh,6fh,6fh,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,PTO,07h,07h,07h,07h,07h,07h,07H,PTO,CrM,CrM,CrM,CrM,CrM,CrM,CrM,CrM,PTO,PTO,04h,04h,04h,07H,07h,07h,07h,07h,07h,07h,07h,07h,6fh
                 DB 07h,07h,07h,PTO,04h,04H,04h,04h,04h,04h,PTO,CrM,CrM,CrM,CrM,PTO,PTO,PTO,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,6fh,6fh
                 DB 07h,07h,PTO,PTO,04h,04h,04H,04h,04h,04h,PTO,CrM,PTO,PTO,PTO,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,04h,6fh,6fh
                 DB 07h,07h,PTO,07h,07h,07h,07h,07H,07h,07h,PTO,PTO,07h,07h,07h,07h,07h,07h,07H,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h
                 ;34x11

LimpaEmanoel     DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 DB 07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07h,07H,07h,07h,07H,07h,07h,07H,07H,07h,07h,07h,07H
                 ;34x11



Pilot            DB BCO,07H,BCO
                 DB 07H,07H,07H
                 DB 07H,07H,07H
                 DB CrD,CrD,CrD
                 DB CrD,CrD,CrD
                 DB CrD,CrD,CrD
                 DB CrD,CrD,CrD
                 DB CrD,CrD,CrD
                 DB 07H,07H,07H
                 ;3x10

LimpaPilot       DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO              
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO
                 DB BCO,BCO,BCO

                 ;3x10
                 
limpaBiroTampa   DB Cra,07h,Cra
                 DB 07h,07h,07h

limpaBiroZ       DB 07h,07h,07h
                 DB 07h,07h,07h
