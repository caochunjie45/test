@echo off
	goto START

:START
    @echo *********************************************
    @echo *   select board                            *
    @echo *********************************************
    @echo  0: ChiHsin_1w500
    @echo  1: TPO_td043m_p2
    @echo  2: 海威讯_5inch
    @echo  3: ChiHsin_1w500_sochip
    @echo  4: ChiHsin_1w500_gadme
    @echo  5: InnoLux_at043
    @echo  6: ococci_lvds
    @echo *********************************************
    
    set /p SEL=Please Select:
    if %SEL%==0     goto LCD0
    if %SEL%==1     goto LCD1
    if %SEL%==2     goto LCD2 
    if %SEL%==3     goto LCD3
    if %SEL%==4     goto LCD4
    if %SEL%==5     goto LCD5
    if %SEL%==6     goto LCD6
    goto ERROR

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD0
	copy lcd_bak\ChiHsin_lw500_0.c lcd0_panel_cfg.c
	copy lcd_bak\ChiHsin_lw500_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD1
	copy lcd_bak\TPO_td043m_p2_0.c lcd0_panel_cfg.c
	copy lcd_bak\TPO_td043m_p2_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD2
	copy lcd_bak\海威讯_5inch_0.c lcd0_panel_cfg.c
	copy lcd_bak\海威讯_5inch_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD3
	copy lcd_bak\ChiHsin_lw500_for_sc2008_0.c lcd0_panel_cfg.c
	copy lcd_bak\ChiHsin_lw500_for_sc2008_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD4
	copy lcd_bak\ChiHsin_lw500_for_gadme_0.c lcd0_panel_cfg.c
	copy lcd_bak\ChiHsin_lw500_for_gadme_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD5
	copy lcd_bak\InnoLux_at043_0.c lcd0_panel_cfg.c
	copy lcd_bak\InnoLux_at043_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LCD6
	copy lcd_bak\ococci_lvds_0.c lcd0_panel_cfg.c
	copy lcd_bak\ococci_lvds_1.c lcd1_panel_cfg.c
    goto conti
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:ERROR
	@echo error selection
	goto conti
    
:conti
    @echo *********************************************
    pause

