
my:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100e8 <exit>:
   100e8:	ff010113          	add	sp,sp,-16
   100ec:	00000593          	li	a1,0
   100f0:	00813023          	sd	s0,0(sp)
   100f4:	00113423          	sd	ra,8(sp)
   100f8:	00050413          	mv	s0,a0
   100fc:	38c000ef          	jal	10488 <__call_exitprocs>
   10100:	f481b503          	ld	a0,-184(gp) # 11e68 <_global_impure_ptr>
   10104:	05853783          	ld	a5,88(a0)
   10108:	00078463          	beqz	a5,10110 <exit+0x28>
   1010c:	000780e7          	jalr	a5
   10110:	00040513          	mv	a0,s0
   10114:	5b4000ef          	jal	106c8 <_exit>

0000000000010118 <main>:
   10118:	fd010113          	add	sp,sp,-48
   1011c:	02813023          	sd	s0,32(sp)
   10120:	01313423          	sd	s3,8(sp)
   10124:	f9818413          	add	s0,gp,-104 # 11eb8 <a>
   10128:	f9818993          	add	s3,gp,-104 # 11eb8 <a>
   1012c:	02113423          	sd	ra,40(sp)
   10130:	00913c23          	sd	s1,24(sp)
   10134:	01213823          	sd	s2,16(sp)
   10138:	f9818713          	add	a4,gp,-104 # 11eb8 <a>
   1013c:	00000793          	li	a5,0
   10140:	01400693          	li	a3,20
   10144:	00f72023          	sw	a5,0(a4)
   10148:	0017879b          	addw	a5,a5,1
   1014c:	00470713          	add	a4,a4,4
   10150:	fed79ae3          	bne	a5,a3,10144 <main+0x2c>
   10154:	05040913          	add	s2,s0,80
   10158:	00040493          	mv	s1,s0
   1015c:	0004a503          	lw	a0,0(s1)
   10160:	00448493          	add	s1,s1,4
   10164:	148000ef          	jal	102ac <print_d>
   10168:	02000513          	li	a0,32
   1016c:	158000ef          	jal	102c4 <print_c>
   10170:	ff2496e3          	bne	s1,s2,1015c <main+0x44>
   10174:	00a00513          	li	a0,10
   10178:	14c000ef          	jal	102c4 <print_c>
   1017c:	01400793          	li	a5,20
   10180:	01300533          	add	a0,zero,s3
   10184:	00f005b3          	add	a1,zero,a5
   10188:	00700893          	li	a7,7
   1018c:	00000073          	ecall
   10190:	00200893          	li	a7,2
   10194:	00000073          	ecall
   10198:	00a00513          	li	a0,10
   1019c:	128000ef          	jal	102c4 <print_c>
   101a0:	00042503          	lw	a0,0(s0)
   101a4:	00440413          	add	s0,s0,4
   101a8:	104000ef          	jal	102ac <print_d>
   101ac:	02000513          	li	a0,32
   101b0:	114000ef          	jal	102c4 <print_c>
   101b4:	ff2416e3          	bne	s0,s2,101a0 <main+0x88>
   101b8:	00a00513          	li	a0,10
   101bc:	108000ef          	jal	102c4 <print_c>
   101c0:	110000ef          	jal	102d0 <exit_proc>
   101c4:	02813083          	ld	ra,40(sp)
   101c8:	02013403          	ld	s0,32(sp)
   101cc:	01813483          	ld	s1,24(sp)
   101d0:	01013903          	ld	s2,16(sp)
   101d4:	00813983          	ld	s3,8(sp)
   101d8:	00000513          	li	a0,0
   101dc:	03010113          	add	sp,sp,48
   101e0:	00008067          	ret

00000000000101e4 <register_fini>:
   101e4:	00000793          	li	a5,0
   101e8:	00078863          	beqz	a5,101f8 <register_fini+0x14>
   101ec:	00010537          	lui	a0,0x10
   101f0:	5b050513          	add	a0,a0,1456 # 105b0 <__libc_fini_array>
   101f4:	4180006f          	j	1060c <atexit>
   101f8:	00008067          	ret

00000000000101fc <_start>:
   101fc:	00002197          	auipc	gp,0x2
   10200:	d2418193          	add	gp,gp,-732 # 11f20 <__global_pointer$>
   10204:	f6018513          	add	a0,gp,-160 # 11e80 <completed.1>
   10208:	fe818613          	add	a2,gp,-24 # 11f08 <__BSS_END__>
   1020c:	40a60633          	sub	a2,a2,a0
   10210:	00000593          	li	a1,0
   10214:	198000ef          	jal	103ac <memset>
   10218:	00000517          	auipc	a0,0x0
   1021c:	3f450513          	add	a0,a0,1012 # 1060c <atexit>
   10220:	00050863          	beqz	a0,10230 <_start+0x34>
   10224:	00000517          	auipc	a0,0x0
   10228:	38c50513          	add	a0,a0,908 # 105b0 <__libc_fini_array>
   1022c:	3e0000ef          	jal	1060c <atexit>
   10230:	0e0000ef          	jal	10310 <__libc_init_array>
   10234:	00012503          	lw	a0,0(sp)
   10238:	00810593          	add	a1,sp,8
   1023c:	00000613          	li	a2,0
   10240:	ed9ff0ef          	jal	10118 <main>
   10244:	ea5ff06f          	j	100e8 <exit>

0000000000010248 <__do_global_dtors_aux>:
   10248:	ff010113          	add	sp,sp,-16
   1024c:	00813023          	sd	s0,0(sp)
   10250:	f601c783          	lbu	a5,-160(gp) # 11e80 <completed.1>
   10254:	00113423          	sd	ra,8(sp)
   10258:	02079263          	bnez	a5,1027c <__do_global_dtors_aux+0x34>
   1025c:	00000793          	li	a5,0
   10260:	00078a63          	beqz	a5,10274 <__do_global_dtors_aux+0x2c>
   10264:	00011537          	lui	a0,0x11
   10268:	70050513          	add	a0,a0,1792 # 11700 <__EH_FRAME_BEGIN__>
   1026c:	00000097          	auipc	ra,0x0
   10270:	000000e7          	jalr	zero # 0 <exit-0x100e8>
   10274:	00100793          	li	a5,1
   10278:	f6f18023          	sb	a5,-160(gp) # 11e80 <completed.1>
   1027c:	00813083          	ld	ra,8(sp)
   10280:	00013403          	ld	s0,0(sp)
   10284:	01010113          	add	sp,sp,16
   10288:	00008067          	ret

000000000001028c <frame_dummy>:
   1028c:	00000793          	li	a5,0
   10290:	00078c63          	beqz	a5,102a8 <frame_dummy+0x1c>
   10294:	00011537          	lui	a0,0x11
   10298:	f6818593          	add	a1,gp,-152 # 11e88 <object.0>
   1029c:	70050513          	add	a0,a0,1792 # 11700 <__EH_FRAME_BEGIN__>
   102a0:	00000317          	auipc	t1,0x0
   102a4:	00000067          	jr	zero # 0 <exit-0x100e8>
   102a8:	00008067          	ret

00000000000102ac <print_d>:
   102ac:	00200893          	li	a7,2
   102b0:	00000073          	ecall
   102b4:	00008067          	ret

00000000000102b8 <print_s>:
   102b8:	00000893          	li	a7,0
   102bc:	00000073          	ecall
   102c0:	00008067          	ret

00000000000102c4 <print_c>:
   102c4:	00100893          	li	a7,1
   102c8:	00000073          	ecall
   102cc:	00008067          	ret

00000000000102d0 <exit_proc>:
   102d0:	00300893          	li	a7,3
   102d4:	00000073          	ecall
   102d8:	00008067          	ret

00000000000102dc <read_char>:
   102dc:	00400893          	li	a7,4
   102e0:	00000073          	ecall
   102e4:	00050513          	mv	a0,a0
   102e8:	0ff57513          	zext.b	a0,a0
   102ec:	00008067          	ret

00000000000102f0 <read_num>:
   102f0:	00500893          	li	a7,5
   102f4:	00000073          	ecall
   102f8:	00050513          	mv	a0,a0
   102fc:	00008067          	ret

0000000000010300 <print_f>:
   10300:	e0050553          	fmv.x.w	a0,fa0
   10304:	00600893          	li	a7,6
   10308:	00000073          	ecall
   1030c:	00008067          	ret

0000000000010310 <__libc_init_array>:
   10310:	fe010113          	add	sp,sp,-32
   10314:	00813823          	sd	s0,16(sp)
   10318:	000117b7          	lui	a5,0x11
   1031c:	00011437          	lui	s0,0x11
   10320:	01213023          	sd	s2,0(sp)
   10324:	70478793          	add	a5,a5,1796 # 11704 <__preinit_array_end>
   10328:	70440713          	add	a4,s0,1796 # 11704 <__preinit_array_end>
   1032c:	00113c23          	sd	ra,24(sp)
   10330:	00913423          	sd	s1,8(sp)
   10334:	40e78933          	sub	s2,a5,a4
   10338:	02e78263          	beq	a5,a4,1035c <__libc_init_array+0x4c>
   1033c:	40395913          	sra	s2,s2,0x3
   10340:	70440413          	add	s0,s0,1796
   10344:	00000493          	li	s1,0
   10348:	00043783          	ld	a5,0(s0)
   1034c:	00148493          	add	s1,s1,1
   10350:	00840413          	add	s0,s0,8
   10354:	000780e7          	jalr	a5
   10358:	ff24e8e3          	bltu	s1,s2,10348 <__libc_init_array+0x38>
   1035c:	00011437          	lui	s0,0x11
   10360:	000117b7          	lui	a5,0x11
   10364:	71878793          	add	a5,a5,1816 # 11718 <__do_global_dtors_aux_fini_array_entry>
   10368:	70840713          	add	a4,s0,1800 # 11708 <__init_array_start>
   1036c:	40e78933          	sub	s2,a5,a4
   10370:	40395913          	sra	s2,s2,0x3
   10374:	02e78063          	beq	a5,a4,10394 <__libc_init_array+0x84>
   10378:	70840413          	add	s0,s0,1800
   1037c:	00000493          	li	s1,0
   10380:	00043783          	ld	a5,0(s0)
   10384:	00148493          	add	s1,s1,1
   10388:	00840413          	add	s0,s0,8
   1038c:	000780e7          	jalr	a5
   10390:	ff24e8e3          	bltu	s1,s2,10380 <__libc_init_array+0x70>
   10394:	01813083          	ld	ra,24(sp)
   10398:	01013403          	ld	s0,16(sp)
   1039c:	00813483          	ld	s1,8(sp)
   103a0:	00013903          	ld	s2,0(sp)
   103a4:	02010113          	add	sp,sp,32
   103a8:	00008067          	ret

00000000000103ac <memset>:
   103ac:	00f00313          	li	t1,15
   103b0:	00050713          	mv	a4,a0
   103b4:	02c37a63          	bgeu	t1,a2,103e8 <memset+0x3c>
   103b8:	00f77793          	and	a5,a4,15
   103bc:	0a079063          	bnez	a5,1045c <memset+0xb0>
   103c0:	06059e63          	bnez	a1,1043c <memset+0x90>
   103c4:	ff067693          	and	a3,a2,-16
   103c8:	00f67613          	and	a2,a2,15
   103cc:	00e686b3          	add	a3,a3,a4
   103d0:	00b73023          	sd	a1,0(a4)
   103d4:	00b73423          	sd	a1,8(a4)
   103d8:	01070713          	add	a4,a4,16
   103dc:	fed76ae3          	bltu	a4,a3,103d0 <memset+0x24>
   103e0:	00061463          	bnez	a2,103e8 <memset+0x3c>
   103e4:	00008067          	ret
   103e8:	40c306b3          	sub	a3,t1,a2
   103ec:	00269693          	sll	a3,a3,0x2
   103f0:	00000297          	auipc	t0,0x0
   103f4:	005686b3          	add	a3,a3,t0
   103f8:	00c68067          	jr	12(a3)
   103fc:	00b70723          	sb	a1,14(a4)
   10400:	00b706a3          	sb	a1,13(a4)
   10404:	00b70623          	sb	a1,12(a4)
   10408:	00b705a3          	sb	a1,11(a4)
   1040c:	00b70523          	sb	a1,10(a4)
   10410:	00b704a3          	sb	a1,9(a4)
   10414:	00b70423          	sb	a1,8(a4)
   10418:	00b703a3          	sb	a1,7(a4)
   1041c:	00b70323          	sb	a1,6(a4)
   10420:	00b702a3          	sb	a1,5(a4)
   10424:	00b70223          	sb	a1,4(a4)
   10428:	00b701a3          	sb	a1,3(a4)
   1042c:	00b70123          	sb	a1,2(a4)
   10430:	00b700a3          	sb	a1,1(a4)
   10434:	00b70023          	sb	a1,0(a4)
   10438:	00008067          	ret
   1043c:	0ff5f593          	zext.b	a1,a1
   10440:	00859693          	sll	a3,a1,0x8
   10444:	00d5e5b3          	or	a1,a1,a3
   10448:	01059693          	sll	a3,a1,0x10
   1044c:	00d5e5b3          	or	a1,a1,a3
   10450:	02059693          	sll	a3,a1,0x20
   10454:	00d5e5b3          	or	a1,a1,a3
   10458:	f6dff06f          	j	103c4 <memset+0x18>
   1045c:	00279693          	sll	a3,a5,0x2
   10460:	00000297          	auipc	t0,0x0
   10464:	005686b3          	add	a3,a3,t0
   10468:	00008293          	mv	t0,ra
   1046c:	f98680e7          	jalr	-104(a3)
   10470:	00028093          	mv	ra,t0
   10474:	ff078793          	add	a5,a5,-16
   10478:	40f70733          	sub	a4,a4,a5
   1047c:	00f60633          	add	a2,a2,a5
   10480:	f6c374e3          	bgeu	t1,a2,103e8 <memset+0x3c>
   10484:	f3dff06f          	j	103c0 <memset+0x14>

0000000000010488 <__call_exitprocs>:
   10488:	fb010113          	add	sp,sp,-80
   1048c:	03413023          	sd	s4,32(sp)
   10490:	f481ba03          	ld	s4,-184(gp) # 11e68 <_global_impure_ptr>
   10494:	03213823          	sd	s2,48(sp)
   10498:	04113423          	sd	ra,72(sp)
   1049c:	1f8a3903          	ld	s2,504(s4)
   104a0:	04813023          	sd	s0,64(sp)
   104a4:	02913c23          	sd	s1,56(sp)
   104a8:	03313423          	sd	s3,40(sp)
   104ac:	01513c23          	sd	s5,24(sp)
   104b0:	01613823          	sd	s6,16(sp)
   104b4:	01713423          	sd	s7,8(sp)
   104b8:	01813023          	sd	s8,0(sp)
   104bc:	04090063          	beqz	s2,104fc <__call_exitprocs+0x74>
   104c0:	00050b13          	mv	s6,a0
   104c4:	00058b93          	mv	s7,a1
   104c8:	00100a93          	li	s5,1
   104cc:	fff00993          	li	s3,-1
   104d0:	00892483          	lw	s1,8(s2)
   104d4:	fff4841b          	addw	s0,s1,-1
   104d8:	02044263          	bltz	s0,104fc <__call_exitprocs+0x74>
   104dc:	00349493          	sll	s1,s1,0x3
   104e0:	009904b3          	add	s1,s2,s1
   104e4:	040b8463          	beqz	s7,1052c <__call_exitprocs+0xa4>
   104e8:	2084b783          	ld	a5,520(s1)
   104ec:	05778063          	beq	a5,s7,1052c <__call_exitprocs+0xa4>
   104f0:	fff4041b          	addw	s0,s0,-1
   104f4:	ff848493          	add	s1,s1,-8
   104f8:	ff3416e3          	bne	s0,s3,104e4 <__call_exitprocs+0x5c>
   104fc:	04813083          	ld	ra,72(sp)
   10500:	04013403          	ld	s0,64(sp)
   10504:	03813483          	ld	s1,56(sp)
   10508:	03013903          	ld	s2,48(sp)
   1050c:	02813983          	ld	s3,40(sp)
   10510:	02013a03          	ld	s4,32(sp)
   10514:	01813a83          	ld	s5,24(sp)
   10518:	01013b03          	ld	s6,16(sp)
   1051c:	00813b83          	ld	s7,8(sp)
   10520:	00013c03          	ld	s8,0(sp)
   10524:	05010113          	add	sp,sp,80
   10528:	00008067          	ret
   1052c:	00892783          	lw	a5,8(s2)
   10530:	0084b703          	ld	a4,8(s1)
   10534:	fff7879b          	addw	a5,a5,-1
   10538:	06878263          	beq	a5,s0,1059c <__call_exitprocs+0x114>
   1053c:	0004b423          	sd	zero,8(s1)
   10540:	fa0708e3          	beqz	a4,104f0 <__call_exitprocs+0x68>
   10544:	31092783          	lw	a5,784(s2)
   10548:	008a96bb          	sllw	a3,s5,s0
   1054c:	00892c03          	lw	s8,8(s2)
   10550:	00d7f7b3          	and	a5,a5,a3
   10554:	0007879b          	sext.w	a5,a5
   10558:	02079263          	bnez	a5,1057c <__call_exitprocs+0xf4>
   1055c:	000700e7          	jalr	a4
   10560:	00892703          	lw	a4,8(s2)
   10564:	1f8a3783          	ld	a5,504(s4)
   10568:	01871463          	bne	a4,s8,10570 <__call_exitprocs+0xe8>
   1056c:	f92782e3          	beq	a5,s2,104f0 <__call_exitprocs+0x68>
   10570:	f80786e3          	beqz	a5,104fc <__call_exitprocs+0x74>
   10574:	00078913          	mv	s2,a5
   10578:	f59ff06f          	j	104d0 <__call_exitprocs+0x48>
   1057c:	31492783          	lw	a5,788(s2)
   10580:	1084b583          	ld	a1,264(s1)
   10584:	00d7f7b3          	and	a5,a5,a3
   10588:	0007879b          	sext.w	a5,a5
   1058c:	00079c63          	bnez	a5,105a4 <__call_exitprocs+0x11c>
   10590:	000b0513          	mv	a0,s6
   10594:	000700e7          	jalr	a4
   10598:	fc9ff06f          	j	10560 <__call_exitprocs+0xd8>
   1059c:	00892423          	sw	s0,8(s2)
   105a0:	fa1ff06f          	j	10540 <__call_exitprocs+0xb8>
   105a4:	00058513          	mv	a0,a1
   105a8:	000700e7          	jalr	a4
   105ac:	fb5ff06f          	j	10560 <__call_exitprocs+0xd8>

00000000000105b0 <__libc_fini_array>:
   105b0:	fe010113          	add	sp,sp,-32
   105b4:	00813823          	sd	s0,16(sp)
   105b8:	000117b7          	lui	a5,0x11
   105bc:	00011437          	lui	s0,0x11
   105c0:	71878793          	add	a5,a5,1816 # 11718 <__do_global_dtors_aux_fini_array_entry>
   105c4:	72040413          	add	s0,s0,1824 # 11720 <impure_data>
   105c8:	40f40433          	sub	s0,s0,a5
   105cc:	00913423          	sd	s1,8(sp)
   105d0:	00113c23          	sd	ra,24(sp)
   105d4:	40345493          	sra	s1,s0,0x3
   105d8:	02048063          	beqz	s1,105f8 <__libc_fini_array+0x48>
   105dc:	ff840413          	add	s0,s0,-8
   105e0:	00f40433          	add	s0,s0,a5
   105e4:	00043783          	ld	a5,0(s0)
   105e8:	fff48493          	add	s1,s1,-1
   105ec:	ff840413          	add	s0,s0,-8
   105f0:	000780e7          	jalr	a5
   105f4:	fe0498e3          	bnez	s1,105e4 <__libc_fini_array+0x34>
   105f8:	01813083          	ld	ra,24(sp)
   105fc:	01013403          	ld	s0,16(sp)
   10600:	00813483          	ld	s1,8(sp)
   10604:	02010113          	add	sp,sp,32
   10608:	00008067          	ret

000000000001060c <atexit>:
   1060c:	00050593          	mv	a1,a0
   10610:	00000693          	li	a3,0
   10614:	00000613          	li	a2,0
   10618:	00000513          	li	a0,0
   1061c:	0040006f          	j	10620 <__register_exitproc>

0000000000010620 <__register_exitproc>:
   10620:	f481b703          	ld	a4,-184(gp) # 11e68 <_global_impure_ptr>
   10624:	1f873783          	ld	a5,504(a4)
   10628:	06078063          	beqz	a5,10688 <__register_exitproc+0x68>
   1062c:	0087a703          	lw	a4,8(a5)
   10630:	01f00813          	li	a6,31
   10634:	08e84663          	blt	a6,a4,106c0 <__register_exitproc+0xa0>
   10638:	02050863          	beqz	a0,10668 <__register_exitproc+0x48>
   1063c:	00371813          	sll	a6,a4,0x3
   10640:	01078833          	add	a6,a5,a6
   10644:	10c83823          	sd	a2,272(a6)
   10648:	3107a883          	lw	a7,784(a5)
   1064c:	00100613          	li	a2,1
   10650:	00e6163b          	sllw	a2,a2,a4
   10654:	00c8e8b3          	or	a7,a7,a2
   10658:	3117a823          	sw	a7,784(a5)
   1065c:	20d83823          	sd	a3,528(a6)
   10660:	00200693          	li	a3,2
   10664:	02d50863          	beq	a0,a3,10694 <__register_exitproc+0x74>
   10668:	00270693          	add	a3,a4,2
   1066c:	00369693          	sll	a3,a3,0x3
   10670:	0017071b          	addw	a4,a4,1
   10674:	00e7a423          	sw	a4,8(a5)
   10678:	00d787b3          	add	a5,a5,a3
   1067c:	00b7b023          	sd	a1,0(a5)
   10680:	00000513          	li	a0,0
   10684:	00008067          	ret
   10688:	20070793          	add	a5,a4,512
   1068c:	1ef73c23          	sd	a5,504(a4)
   10690:	f9dff06f          	j	1062c <__register_exitproc+0xc>
   10694:	3147a683          	lw	a3,788(a5)
   10698:	00000513          	li	a0,0
   1069c:	00c6e6b3          	or	a3,a3,a2
   106a0:	30d7aa23          	sw	a3,788(a5)
   106a4:	00270693          	add	a3,a4,2
   106a8:	00369693          	sll	a3,a3,0x3
   106ac:	0017071b          	addw	a4,a4,1
   106b0:	00e7a423          	sw	a4,8(a5)
   106b4:	00d787b3          	add	a5,a5,a3
   106b8:	00b7b023          	sd	a1,0(a5)
   106bc:	00008067          	ret
   106c0:	fff00513          	li	a0,-1
   106c4:	00008067          	ret

00000000000106c8 <_exit>:
   106c8:	05d00893          	li	a7,93
   106cc:	00000073          	ecall
   106d0:	00054463          	bltz	a0,106d8 <_exit+0x10>
   106d4:	0000006f          	j	106d4 <_exit+0xc>
   106d8:	ff010113          	add	sp,sp,-16
   106dc:	00813023          	sd	s0,0(sp)
   106e0:	00050413          	mv	s0,a0
   106e4:	00113423          	sd	ra,8(sp)
   106e8:	4080043b          	negw	s0,s0
   106ec:	00c000ef          	jal	106f8 <__errno>
   106f0:	00852023          	sw	s0,0(a0)
   106f4:	0000006f          	j	106f4 <_exit+0x2c>

00000000000106f8 <__errno>:
   106f8:	f581b503          	ld	a0,-168(gp) # 11e78 <_impure_ptr>
   106fc:	00008067          	ret
