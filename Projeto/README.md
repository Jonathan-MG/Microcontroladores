# Projeto MicroControladores - Batmóvel dos bombeiros controlado por bluetooth
- Professor: Sergio Ribeiro 

## Integrantes do Grupo 
- Gabriel Prande Bernardello    RA: 20.01288-8
- Jonathan Martins Gomes        RA: 20.00862-7
- Matheus Marins Bernardello    RA: 20.00286-6

## Objetivo do projeto 

A ideia principal do projeto seria um carrinho que pudessemos controlar via bluetooth, com isso implementados a ideia usando o o Raspberry Pi Pico como controlador do mesmo, adotamos o micro-servo para darmos direções ao carro e não somente frente-trás, colocamos uma bomba de água para o mesmo poder atender os chamados de Gotham City por completo. 
Nosso Batmóvel é super completo e pode ter velocidade variada de acordo com a carga da bateria, portanto se deseja maior eficiencia matenha-o com carga, para possível troca de bateria favor consultar esquema elétrico do mesmo para não explodir o carro!
Como requisitos deste projeto tinhamos: 
- Realizar a conectividade do celular com o HC05 - bluetooth 
- Ligar o bluetooth no raspberry para o mesmo dar comandos aos motores
- Fazer com que os motores funcionem através da ponte H
- Bomba de água ser acionada somente comando acionado
- OBS: Códigos usados estão disponíveis em suas respectivas pastas, favor consultar este mesmo git! Para a implementação do mesmo foi usado a IDE Thonny, conforme documentação oficial Raspberry recomenda. 

[Veja só como ficou!!](https://youtu.be/vZS8VveIKf4)
[Veja ele funcionando na faculdade!!](https://youtu.be/E4jR8mIG90I)


### Custos do projeto 
**Materiais usados:**
- Raspberry PI PICO - R$ 49,90 + Frete [Mercado Livre](https://produto.mercadolivre.com.br/MLB-3112865208-placa-raspberry-pi-pico-pronta-entrega-_JM?matt_tool=40343894&matt_word=&matt_source=google&matt_campaign_id=14303413655&matt_ad_group_id=133855953276&matt_match_type=&matt_network=g&matt_device=c&matt_creative=584156655519&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=683141687&matt_product_id=MLB3112865208&matt_product_partition_id=1816238256222&matt_target_id=aud-1966009190540:pla-1816238256222&gclid=Cj0KCQjw7PCjBhDwARIsANo7CgkmehwrmQquv8RtEMShWC1YGl8gpfhthpJWBV20UmKZsy4QnM3t3bsaApMmEALw_wcB)

- Módulo bluetooth Master HC05 - R$ 30,51 + Frete [Curto Circuito](https://curtocircuito.com.br/modulo-bluetooth-master-hc05.html?utm_term=&utm_campaign=&utm_source=adwords&utm_medium=ppc&hsa_acc=7016354091&hsa_cam=20120617836&hsa_grp=146883402697&hsa_ad=658348170880&hsa_src=g&hsa_tgt=pla-1513250356744&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3&gclid=Cj0KCQjw7PCjBhDwARIsANo7Cglzpr6OAQ8jeTGvstxQfQD54pw-qoCu-7GztwpsirBtI1xdC6O5EmsaAto7EALw_wcB) 

- Mini Bomba de água 5v - R$ 67,70 + Frete [Amazon](https://www.amazon.com.br/Queenser-bomba-submergível-silenciosa-elevador/dp/B09C8KM352/ref=asc_df_B09C8KM352/?tag=googleshopp00-20&linkCode=df0&hvadid=379714841427&hvpos=&hvnetw=g&hvrand=5834169608499349378&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1001767&hvtargid=pla-1449770690808&psc=1)

- Protoboard 830 pontos - R$ 11,61 + Frete [Curto Circuito](https://curtocircuito.com.br/protoboard-830-pontos.html?utm_term=&utm_campaign=&utm_source=adwords&utm_medium=ppc&hsa_acc=7016354091&hsa_cam=18092179832&hsa_grp=139707858905&hsa_ad=617274207530&hsa_src=g&hsa_tgt=pla-379661118706&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3&gclid=Cj0KCQjw7PCjBhDwARIsANo7Cgl6eE_Ar54K0S3-FHv1Gvf_Fu6J3yYM-8drIweWrWIdOsKZRHgqrE0aAqRlEALw_wcB)

- Jumpers - R$ 24,00 + Frete [Mercado Livre](https://produto.mercadolivre.com.br/MLB-1021476692-kit-jumper-20cm-60-pecas-mxf-mxm-fxf-arduino-cabinho-wire-_JM#is_advertising=true&position=3&search_layout=grid&type=pad&tracking_id=c1fc22c3-26a5-45ac-9907-e5d6f22fa532&is_advertising=true&ad_domain=VQCATCORE_LST&ad_position=3&ad_click_id=MzQ0OWFmZjYtMGIyYS00ZTQ2LThlMjctYWUyZDUyYzJmMWJl)

- Kit carrinho MDF - R$ 187,75 + Frete [UsinaInfo](https://www.usinainfo.com.br/kit-robotica/carrinho-arduino-mdf-4wd-200rpm-v2-com-micro-servo-motor-e-eixo-movel-kit-chassi-manual-de-montagem-4846.html)

- Módulo Driver Ponte H - R$ 19,00 + Frete [Mercado Livre](https://produto.mercadolivre.com.br/MLB-1954738598-modulo-driver-ponte-h-l298n-l298-_JM?matt_tool=40343894&matt_word=&matt_source=google&matt_campaign_id=14303413655&matt_ad_group_id=133855953276&matt_match_type=&matt_network=g&matt_device=c&matt_creative=584156655519&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=687433387&matt_product_id=MLB1954738598&matt_product_partition_id=1801030559419&matt_target_id=aud-1966009190540:pla-1801030559419&gclid=Cj0KCQjw7PCjBhDwARIsANo7CgkuPJaqAIdNTUEV2nlsV4KBbKwOzwXezCXHSzuFMTpDsP1FfJgKwckaAuY2EALw_wcB)

- Relé modular - R$ 4,95 + Frete [Curto Circuito](https://curtocircuito.com.br/modulo-rele-5v-10a-1-canal.html?utm_term=&utm_campaign=&utm_source=adwords&utm_medium=ppc&hsa_acc=7016354091&hsa_cam=15492204693&hsa_grp=133699292634&hsa_ad=567463285566&hsa_src=g&hsa_tgt=pla-1513250355344&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3&gclid=Cj0KCQjw7PCjBhDwARIsANo7CgkhQQXyi93agv2hKwMZSVtkBAQ_93oMOYfg1mLfjzL97F2QIl078QsaAimHEALw_wcB)

- Conversor de nível lógico - R$ 12,22 + Frete [Mercado Livre](https://produto.mercadolivre.com.br/MLB-1109659627-conversor-de-nivel-logico-i2c-33-5v-bidirecional-_JM?matt_tool=40343894&matt_word=&matt_source=google&matt_campaign_id=14303413655&matt_ad_group_id=133855953276&matt_match_type=&matt_network=g&matt_device=c&matt_creative=584156655519&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=306483302&matt_product_id=MLB1109659627&matt_product_partition_id=1816238256222&matt_target_id=aud-1966009190540:pla-1816238256222&gclid=Cj0KCQjw7PCjBhDwARIsANo7CgkUntVl_Pm9UT31zOXigIVAVss4Ht8FlzYBs8XDG-eZtXmA3kMaMj0aAjCnEALw_wcB)

- Bateria recarregável 3x - R$ 15,90 [Leroy Merlin](https://www.leroymerlin.com.br/bateria-li-ion-18650-6800mah-3-7v-recarregavel_1571036170?region=outros&gclid=Cj0KCQjw7PCjBhDwARIsANo7Cgm3fb9RE-DoZj841HlZLK-m85ldd46PftG2A1n_4ipDkV6OXRQiTGYaAnKEEALw_wcB)

- Suporte para bateria recarregável - 12,40 + Frete [Loja da eletrônica](https://www.lojadarobotica.com.br/suporte-de-bateria-18650-para-3-baterias?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&gclid=Cj0KCQjw7PCjBhDwARIsANo7CglmI22-X1Hgk5cILHv0flF3SEFqbkzA1oO3JdJJ5mQ_cLUA-OlPCfEaAjfGEALw_wcB)

- Copos para reservatório água - R$ 36,00 - Comprado em loja física

- Placa de madeira MDF 3mm para confecção de suporte copo - R$ 1,75 + Frete [Figueira Laser](https://www.figueiralaser.com.br/ver-recorte-personalizado/recorte-quadrado-ou-retangulo/3/mdf-3-mm/15.00-x-15.00-cm)

- **_Somando os itens, projeto fechado em R$ 505,49._**

