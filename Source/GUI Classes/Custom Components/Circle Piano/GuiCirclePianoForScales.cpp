//
//  GuiCirclePianoForScales.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 10/01/2012.
//  Copyright 2012 nu desine.
//
//  This file is part of AlphaLive.
//
//  AlphaLive is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2, 
//  as published by the Free Software Foundation.
//  
//  AlphaLive is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "GuiCirclePianoForScales.h"
#include "../../Binary Data/CirclePianoBinaryData.h"
#include "../../../File and Settings/AppSettings.h"
#include "../../../Functionality Classes/Other/LayoutsAndScales.cpp"
#include "../../../File and Settings/StoredSettings.h"

#define PAD_SETTINGS AppSettings::Instance()->padSettings[currentlySelectedPad]
#define PAD_SETTINGS_i AppSettings::Instance()->padSettings[i]

GuiCirclePianoForScales::GuiCirclePianoForScales() : Component ("GuiCirclePianoForScales")

{
    //background graphics
	image = ImageFileFormat::loadFrom(CirclePianoBinaryData::circlepiano_png, CirclePianoBinaryData::circlepiano_pngSize);
    
    //note display label
    addAndMakeVisible(midiNoteLabel = new Label("Note Label", "60"));
    midiNoteLabel->setJustificationType(Justification::horizontallyCentred);
    midiNoteLabel->setColour(Label::textColourId, Colours::lightgrey);
    //Font newFont;
    //newFont.setTypefaceName("Grixel Acme 7 Wide");
    //midiNoteLabel->setFont(newFont);
    midiNoteLabel->addMouseListener(this, true);
    
    //------------------scale combobox------------------------------
    addAndMakeVisible(scaleMenu = new ComboBox());
    scaleMenu->addListener(this);
    scaleMenu->addMouseListener(this, true);
    
    scaleMenu->addItem("None Selected...", 100);
    scaleMenu->addItem("Chromatic", 1);
    scaleMenu->addItem("Major", 2);
    scaleMenu->addItem("Major Pentatonic", 3);
    scaleMenu->addItem("Minor", 4);
    scaleMenu->addItem("Minor Pentatonic", 5);
    scaleMenu->addItem("Melodic Minor", 6);
    scaleMenu->addItem("Harmonic Minor", 7);
    scaleMenu->addItem("Blues", 8);
    scaleMenu->addItem("Rock n' Roll", 9);
    scaleMenu->addItem("Hungarian Gypsy", 10);
    scaleMenu->addItem("Raga Madhuri", 11);
    
    scaleMenu->setSelectedId(100, true);
	
    //THESE FILES NEED TO BE LOADED INTO MEMORY USING IMAGECACHE
	
	keyOffIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key0off_png, CirclePianoBinaryData::key0off_pngSize)));
	keyOffIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key1off_png, CirclePianoBinaryData::key1off_pngSize)));
	keyOffIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key2off_png, CirclePianoBinaryData::key2off_pngSize)));
	keyOffIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key3off_png, CirclePianoBinaryData::key3off_pngSize)));
	keyOffIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key4off_png, CirclePianoBinaryData::key4off_pngSize)));
	keyOffIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key5off_png, CirclePianoBinaryData::key5off_pngSize)));
	keyOffIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key6off_png, CirclePianoBinaryData::key6off_pngSize)));
	keyOffIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key7off_png, CirclePianoBinaryData::key7off_pngSize)));
	keyOffIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key8off_png, CirclePianoBinaryData::key8off_pngSize)));
	keyOffIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key9off_png, CirclePianoBinaryData::key9off_pngSize)));
	keyOffIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key10off_png, CirclePianoBinaryData::key10off_pngSize)));
	keyOffIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key11off_png, CirclePianoBinaryData::key11off_pngSize)));
	keyOffIm.insert(12, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key12off_png, CirclePianoBinaryData::key12off_pngSize)));
	keyOffIm.insert(13, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key13off_png, CirclePianoBinaryData::key13off_pngSize)));
    keyOffIm.insert(14, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key14off_png, CirclePianoBinaryData::key14off_pngSize)));
	keyOffIm.insert(15, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key15off_png, CirclePianoBinaryData::key15off_pngSize)));
	keyOffIm.insert(16, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key16off_png, CirclePianoBinaryData::key16off_pngSize)));
	keyOffIm.insert(17, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key17off_png, CirclePianoBinaryData::key17off_pngSize)));
	keyOffIm.insert(18, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key18off_png, CirclePianoBinaryData::key18off_pngSize)));
	keyOffIm.insert(19, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key19off_png, CirclePianoBinaryData::key19off_pngSize)));
	keyOffIm.insert(20, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key20off_png, CirclePianoBinaryData::key20off_pngSize)));
    keyOffIm.insert(21, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key21off_png, CirclePianoBinaryData::key21off_pngSize)));
	keyOffIm.insert(22, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key22off_png, CirclePianoBinaryData::key22off_pngSize)));
	keyOffIm.insert(23, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key23off_png, CirclePianoBinaryData::key23off_pngSize)));
	keyOffIm.insert(24, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key24off_png, CirclePianoBinaryData::key24off_pngSize)));
	keyOffIm.insert(25, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key25off_png, CirclePianoBinaryData::key25off_pngSize)));
	keyOffIm.insert(26, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key26off_png, CirclePianoBinaryData::key26off_pngSize)));
	keyOffIm.insert(27, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key27off_png, CirclePianoBinaryData::key27off_pngSize)));
	keyOffIm.insert(28, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key28off_png, CirclePianoBinaryData::key28off_pngSize)));
	keyOffIm.insert(29, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key29off_png, CirclePianoBinaryData::key29off_pngSize)));
	keyOffIm.insert(30, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key30off_png, CirclePianoBinaryData::key30off_pngSize)));
	keyOffIm.insert(31, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key31off_png, CirclePianoBinaryData::key31off_pngSize)));
	keyOffIm.insert(32, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key32off_png, CirclePianoBinaryData::key32off_pngSize)));
	keyOffIm.insert(33, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key33off_png, CirclePianoBinaryData::key33off_pngSize)));
	keyOffIm.insert(34, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key34off_png, CirclePianoBinaryData::key34off_pngSize)));
	keyOffIm.insert(35, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key35off_png, CirclePianoBinaryData::key35off_pngSize)));
    keyOffIm.insert(36, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key36off_png, CirclePianoBinaryData::key36off_pngSize)));
	keyOffIm.insert(37, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key37off_png, CirclePianoBinaryData::key37off_pngSize)));
	keyOffIm.insert(38, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key38off_png, CirclePianoBinaryData::key38off_pngSize)));
    keyOffIm.insert(39, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key39off_png, CirclePianoBinaryData::key39off_pngSize)));
	keyOffIm.insert(40, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key40off_png, CirclePianoBinaryData::key40off_pngSize)));
	keyOffIm.insert(41, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key41off_png, CirclePianoBinaryData::key41off_pngSize)));
	keyOffIm.insert(42, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key42off_png, CirclePianoBinaryData::key42off_pngSize)));
	keyOffIm.insert(43, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key43off_png, CirclePianoBinaryData::key43off_pngSize)));
	keyOffIm.insert(44, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key44off_png, CirclePianoBinaryData::key44off_pngSize)));
	keyOffIm.insert(45, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key45off_png, CirclePianoBinaryData::key45off_pngSize)));
	keyOffIm.insert(46, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key46off_png, CirclePianoBinaryData::key46off_pngSize)));
    keyOffIm.insert(47, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key47off_png, CirclePianoBinaryData::key47off_pngSize)));
	
    
	keyOverIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key0over_png, CirclePianoBinaryData::key0over_pngSize)));
	keyOverIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key1over_png, CirclePianoBinaryData::key1over_pngSize)));
	keyOverIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key2over_png, CirclePianoBinaryData::key2over_pngSize)));
	keyOverIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key3over_png, CirclePianoBinaryData::key3over_pngSize)));
	keyOverIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key4over_png, CirclePianoBinaryData::key4over_pngSize)));
	keyOverIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key5over_png, CirclePianoBinaryData::key5over_pngSize)));
	keyOverIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key6over_png, CirclePianoBinaryData::key6over_pngSize)));
	keyOverIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key7over_png, CirclePianoBinaryData::key7over_pngSize)));
	keyOverIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key8over_png, CirclePianoBinaryData::key8over_pngSize)));
	keyOverIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key9over_png, CirclePianoBinaryData::key9over_pngSize)));
    keyOverIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key10over_png, CirclePianoBinaryData::key10over_pngSize)));
	keyOverIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key11over_png, CirclePianoBinaryData::key11over_pngSize)));
	keyOverIm.insert(12, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key12over_png, CirclePianoBinaryData::key12over_pngSize)));
    keyOverIm.insert(13, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key13over_png, CirclePianoBinaryData::key13over_pngSize)));
	keyOverIm.insert(14, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key14over_png, CirclePianoBinaryData::key14over_pngSize)));
	keyOverIm.insert(15, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key15over_png, CirclePianoBinaryData::key15over_pngSize)));
	keyOverIm.insert(16, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key16over_png, CirclePianoBinaryData::key16over_pngSize)));
	keyOverIm.insert(17, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key17over_png, CirclePianoBinaryData::key17over_pngSize)));
	keyOverIm.insert(18, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key18over_png, CirclePianoBinaryData::key18over_pngSize)));
	keyOverIm.insert(19, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key19over_png, CirclePianoBinaryData::key19over_pngSize)));
	keyOverIm.insert(20, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key20over_png, CirclePianoBinaryData::key20over_pngSize)));
	keyOverIm.insert(21, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key21over_png, CirclePianoBinaryData::key21over_pngSize)));
	keyOverIm.insert(22, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key22over_png, CirclePianoBinaryData::key22over_pngSize)));
	keyOverIm.insert(23, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key23over_png, CirclePianoBinaryData::key23over_pngSize)));
    keyOverIm.insert(24, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key24over_png, CirclePianoBinaryData::key24over_pngSize)));
    keyOverIm.insert(25, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key25over_png, CirclePianoBinaryData::key25over_pngSize)));
	keyOverIm.insert(26, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key26over_png, CirclePianoBinaryData::key26over_pngSize)));
	keyOverIm.insert(27, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key27over_png, CirclePianoBinaryData::key27over_pngSize)));
	keyOverIm.insert(28, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key28over_png, CirclePianoBinaryData::key28over_pngSize)));
	keyOverIm.insert(29, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key29over_png, CirclePianoBinaryData::key29over_pngSize)));
	keyOverIm.insert(30, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key30over_png, CirclePianoBinaryData::key30over_pngSize)));
	keyOverIm.insert(31, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key31over_png, CirclePianoBinaryData::key31over_pngSize)));
	keyOverIm.insert(32, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key32over_png, CirclePianoBinaryData::key32over_pngSize)));
	keyOverIm.insert(33, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key33over_png, CirclePianoBinaryData::key33over_pngSize)));
	keyOverIm.insert(34, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key34over_png, CirclePianoBinaryData::key34over_pngSize)));
	keyOverIm.insert(35, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key35over_png, CirclePianoBinaryData::key35over_pngSize)));
	keyOverIm.insert(36, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key36over_png, CirclePianoBinaryData::key36over_pngSize)));
	keyOverIm.insert(37, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key37over_png, CirclePianoBinaryData::key37over_pngSize)));
	keyOverIm.insert(38, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key38over_png, CirclePianoBinaryData::key38over_pngSize)));
	keyOverIm.insert(39, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key39over_png, CirclePianoBinaryData::key39over_pngSize)));
	keyOverIm.insert(40, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key40over_png, CirclePianoBinaryData::key40over_pngSize)));
	keyOverIm.insert(41, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key41over_png, CirclePianoBinaryData::key41over_pngSize)));
	keyOverIm.insert(42, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key42over_png, CirclePianoBinaryData::key42over_pngSize)));
	keyOverIm.insert(43, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key43over_png, CirclePianoBinaryData::key43over_pngSize)));
	keyOverIm.insert(44, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key44over_png, CirclePianoBinaryData::key44over_pngSize)));
	keyOverIm.insert(45, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key45over_png, CirclePianoBinaryData::key45over_pngSize)));
	keyOverIm.insert(46, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key46over_png, CirclePianoBinaryData::key46over_pngSize)));
	keyOverIm.insert(47, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key47over_png, CirclePianoBinaryData::key47over_pngSize)));
    
    
	keyDownIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key0down_png, CirclePianoBinaryData::key0down_pngSize)));
	keyDownIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key1down_png, CirclePianoBinaryData::key1down_pngSize)));
	keyDownIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key2down_png, CirclePianoBinaryData::key2down_pngSize)));
	keyDownIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key3down_png, CirclePianoBinaryData::key3down_pngSize)));
	keyDownIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key4down_png, CirclePianoBinaryData::key4down_pngSize)));
	keyDownIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key5down_png, CirclePianoBinaryData::key5down_pngSize)));
	keyDownIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key6down_png, CirclePianoBinaryData::key6down_pngSize)));
	keyDownIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key7down_png, CirclePianoBinaryData::key7down_pngSize)));
	keyDownIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key8down_png, CirclePianoBinaryData::key8down_pngSize)));
	keyDownIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key9down_png, CirclePianoBinaryData::key9down_pngSize)));
	keyDownIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key10down_png, CirclePianoBinaryData::key10down_pngSize)));
	keyDownIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key11down_png, CirclePianoBinaryData::key11down_pngSize)));
	keyDownIm.insert(12, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key12down_png, CirclePianoBinaryData::key12down_pngSize)));
	keyDownIm.insert(13, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key13down_png, CirclePianoBinaryData::key13down_pngSize)));
	keyDownIm.insert(14, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key14down_png, CirclePianoBinaryData::key14down_pngSize)));
	keyDownIm.insert(15, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key15down_png, CirclePianoBinaryData::key15down_pngSize)));
	keyDownIm.insert(16, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key16down_png, CirclePianoBinaryData::key16down_pngSize)));
    keyDownIm.insert(17, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key17down_png, CirclePianoBinaryData::key17down_pngSize)));
	keyDownIm.insert(18, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key18down_png, CirclePianoBinaryData::key18down_pngSize)));
	keyDownIm.insert(19, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key19down_png, CirclePianoBinaryData::key19down_pngSize)));
    keyDownIm.insert(20, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key20down_png, CirclePianoBinaryData::key20down_pngSize)));
	keyDownIm.insert(21, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key21down_png, CirclePianoBinaryData::key21down_pngSize)));
	keyDownIm.insert(22, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key22down_png, CirclePianoBinaryData::key22down_pngSize)));
	keyDownIm.insert(23, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key23down_png, CirclePianoBinaryData::key23down_pngSize)));
	keyDownIm.insert(24, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key24down_png, CirclePianoBinaryData::key24down_pngSize)));
	keyDownIm.insert(25, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key25down_png, CirclePianoBinaryData::key25down_pngSize)));
	keyDownIm.insert(26, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key26down_png, CirclePianoBinaryData::key26down_pngSize)));
	keyDownIm.insert(27, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key27down_png, CirclePianoBinaryData::key27down_pngSize)));
	keyDownIm.insert(28, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key28down_png, CirclePianoBinaryData::key28down_pngSize)));
	keyDownIm.insert(29, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key29down_png, CirclePianoBinaryData::key29down_pngSize)));
	keyDownIm.insert(30, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key30down_png, CirclePianoBinaryData::key30down_pngSize)));
	keyDownIm.insert(31, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key31down_png, CirclePianoBinaryData::key31down_pngSize)));
	keyDownIm.insert(32, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key32down_png, CirclePianoBinaryData::key32down_pngSize)));
    keyDownIm.insert(33, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key33down_png, CirclePianoBinaryData::key33down_pngSize)));
    keyDownIm.insert(34, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key34down_png, CirclePianoBinaryData::key34down_pngSize)));
	keyDownIm.insert(35, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key35down_png, CirclePianoBinaryData::key35down_pngSize)));
	keyDownIm.insert(36, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key36down_png, CirclePianoBinaryData::key36down_pngSize)));
	keyDownIm.insert(37, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key37down_png, CirclePianoBinaryData::key37down_pngSize)));
	keyDownIm.insert(38, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key38down_png, CirclePianoBinaryData::key38down_pngSize)));
	keyDownIm.insert(39, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key39down_png, CirclePianoBinaryData::key39down_pngSize)));
	keyDownIm.insert(40, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key40down_png, CirclePianoBinaryData::key40down_pngSize)));
	keyDownIm.insert(41, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key41down_png, CirclePianoBinaryData::key41down_pngSize)));
	keyDownIm.insert(42, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key42down_png, CirclePianoBinaryData::key42down_pngSize)));
	keyDownIm.insert(43, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key43down_png, CirclePianoBinaryData::key43down_pngSize)));
	keyDownIm.insert(44, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key44down_png, CirclePianoBinaryData::key44down_pngSize)));
	keyDownIm.insert(45, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key45down_png, CirclePianoBinaryData::key45down_pngSize)));
	keyDownIm.insert(46, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key46down_png, CirclePianoBinaryData::key46down_pngSize)));
	keyDownIm.insert(47, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key47down_png, CirclePianoBinaryData::key47down_pngSize)));
	
    
    keyOnIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key0on_png, CirclePianoBinaryData::key0on_pngSize)));
	keyOnIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key1on_png, CirclePianoBinaryData::key1on_pngSize)));
	keyOnIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key2on_png, CirclePianoBinaryData::key2on_pngSize)));
	keyOnIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key3on_png, CirclePianoBinaryData::key3on_pngSize)));
	keyOnIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key4on_png, CirclePianoBinaryData::key4on_pngSize)));
    keyOnIm.insert(5, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key5on_png, CirclePianoBinaryData::key5on_pngSize)));
	keyOnIm.insert(6, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key6on_png, CirclePianoBinaryData::key6on_pngSize)));
	keyOnIm.insert(7, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key7on_png, CirclePianoBinaryData::key7on_pngSize)));
	keyOnIm.insert(8, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key8on_png, CirclePianoBinaryData::key8on_pngSize)));
	keyOnIm.insert(9, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key9on_png, CirclePianoBinaryData::key9on_pngSize)));
	keyOnIm.insert(10, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key10on_png, CirclePianoBinaryData::key10on_pngSize)));
	keyOnIm.insert(11, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key11on_png, CirclePianoBinaryData::key11on_pngSize)));
	keyOnIm.insert(12, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key12on_png, CirclePianoBinaryData::key12on_pngSize)));
	keyOnIm.insert(13, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key13on_png, CirclePianoBinaryData::key13on_pngSize)));
	keyOnIm.insert(14, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key14on_png, CirclePianoBinaryData::key14on_pngSize)));
	keyOnIm.insert(15, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key15on_png, CirclePianoBinaryData::key15on_pngSize)));
    keyOnIm.insert(16, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key16on_png, CirclePianoBinaryData::key16on_pngSize)));
	keyOnIm.insert(17, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key17on_png, CirclePianoBinaryData::key17on_pngSize)));
	keyOnIm.insert(18, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key18on_png, CirclePianoBinaryData::key18on_pngSize)));
	keyOnIm.insert(19, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key19on_png, CirclePianoBinaryData::key19on_pngSize)));
	keyOnIm.insert(20, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key20on_png, CirclePianoBinaryData::key20on_pngSize)));
	keyOnIm.insert(21, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key21on_png, CirclePianoBinaryData::key21on_pngSize)));
	keyOnIm.insert(22, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key22on_png, CirclePianoBinaryData::key22on_pngSize)));
	keyOnIm.insert(23, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key23on_png, CirclePianoBinaryData::key23on_pngSize)));
	keyOnIm.insert(24, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key24on_png, CirclePianoBinaryData::key24on_pngSize)));
	keyOnIm.insert(25, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key25on_png, CirclePianoBinaryData::key25on_pngSize)));
	keyOnIm.insert(26, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key26on_png, CirclePianoBinaryData::key26on_pngSize)));
	keyOnIm.insert(27, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key27on_png, CirclePianoBinaryData::key27on_pngSize)));
	keyOnIm.insert(28, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key28on_png, CirclePianoBinaryData::key28on_pngSize)));
	keyOnIm.insert(29, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key29on_png, CirclePianoBinaryData::key29on_pngSize)));
	keyOnIm.insert(30, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key30on_png, CirclePianoBinaryData::key30on_pngSize)));
	keyOnIm.insert(31, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key31on_png, CirclePianoBinaryData::key31on_pngSize)));
	keyOnIm.insert(32, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key32on_png, CirclePianoBinaryData::key32on_pngSize)));
	keyOnIm.insert(33, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key33on_png, CirclePianoBinaryData::key33on_pngSize)));
	keyOnIm.insert(34, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key34on_png, CirclePianoBinaryData::key34on_pngSize)));
	keyOnIm.insert(35, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key35on_png, CirclePianoBinaryData::key35on_pngSize)));
	keyOnIm.insert(36, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key36on_png, CirclePianoBinaryData::key36on_pngSize)));
	keyOnIm.insert(37, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key37on_png, CirclePianoBinaryData::key37on_pngSize)));
	keyOnIm.insert(38, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key38on_png, CirclePianoBinaryData::key38on_pngSize)));
	keyOnIm.insert(39, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key39on_png, CirclePianoBinaryData::key39on_pngSize)));
	keyOnIm.insert(40, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key40on_png, CirclePianoBinaryData::key40on_pngSize)));
	keyOnIm.insert(41, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key41on_png, CirclePianoBinaryData::key41on_pngSize)));
	keyOnIm.insert(42, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key42on_png, CirclePianoBinaryData::key42on_pngSize)));
	keyOnIm.insert(43, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key43on_png, CirclePianoBinaryData::key43on_pngSize)));
	keyOnIm.insert(44, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key44on_png, CirclePianoBinaryData::key44on_pngSize)));
	keyOnIm.insert(45, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key45on_png, CirclePianoBinaryData::key45on_pngSize)));
	keyOnIm.insert(46, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key46on_png, CirclePianoBinaryData::key46on_pngSize)));
	keyOnIm.insert(47, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::key47on_png, CirclePianoBinaryData::key47on_pngSize)));
	
	
	transposeOffIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b0off_png, CirclePianoBinaryData::b0off_pngSize)));
	transposeOffIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b1off_png, CirclePianoBinaryData::b1off_pngSize)));
	transposeOffIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b2off_png, CirclePianoBinaryData::b2off_pngSize)));
	transposeOffIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b3off_png, CirclePianoBinaryData::b3off_pngSize)));
	transposeOffIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b4off_png, CirclePianoBinaryData::b4off_pngSize)));
	
	transposeOverIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b0over_png, CirclePianoBinaryData::b0over_pngSize)));
	transposeOverIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b1over_png, CirclePianoBinaryData::b1over_pngSize)));
	transposeOverIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b2over_png, CirclePianoBinaryData::b2over_pngSize)));
	transposeOverIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b3over_png, CirclePianoBinaryData::b3over_pngSize)));
	transposeOverIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b4over_png, CirclePianoBinaryData::b4over_pngSize)));
	
	transposeDownIm.insert(0, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b0down_png, CirclePianoBinaryData::b0down_pngSize)));
	transposeDownIm.insert(1, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b1down_png, CirclePianoBinaryData::b1down_pngSize)));
	transposeDownIm.insert(2, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b2down_png, CirclePianoBinaryData::b2down_pngSize)));
	transposeDownIm.insert(3, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b3down_png, CirclePianoBinaryData::b3down_pngSize)));
	transposeDownIm.insert(4, new Image(ImageFileFormat::loadFrom(CirclePianoBinaryData::b4down_png, CirclePianoBinaryData::b4down_pngSize)));
	
	for (int i = 0; i <= 4; i++)
    {
		buttons.insert(i, new GuiFxButtons(transposeOffIm[i], transposeOverIm[i], transposeDownIm[i]));
		addAndMakeVisible(buttons[i]);
		buttons[i]->setRadioGroupId (600);
		buttons[i]->addListener(this);
		buttons[i]->setOpaque(false);
	}
    
    //set '0' transpose as default
    buttons[2]->setToggleState(true, false);
    transposeValue = 3;
    
	
	for (int i = 0; i <= 47; i++) 
    {
		keys.insert(i, new GuiPianoKeys(keyOffIm[i], keyOverIm[i], keyDownIm[i], keyOnIm[i]));
		addAndMakeVisible(keys[i]);
        //DONT SET BUTTONS AS A RADIO GROUP
        //as we want to display all notes of a scale on the piano there will
        //be certain situations where more than one key can be down at a time.
        //However when clicked on I'll need to use a manual method for deselecting
        //any keys which are currently pressed.
		//keys[i]->setRadioGroupId (12);
		keys[i]->addListener(this);
        keys[i]->addMouseListener(this, true);
		keys[i]->setOpaque(false);
	}
    
    //set 60 (middle C) as default
    keys[24]->setToggleState(true, false);
    keyValue = 24;
	rootNote = 60;
    
    noteDisplayType = StoredSettings::getInstance()->midiNoteDisplayType;
	
	hitPath.addEllipse (0.0f, 0.0f, 230.0f, 230.0f);
    
}

GuiCirclePianoForScales::~GuiCirclePianoForScales()
{
    buttons.clear();
    keys.clear();
	deleteAllChildren();
}


void GuiCirclePianoForScales::resized()
{
	hitPath.addEllipse(0.0f, 0.0f, getWidth(), getHeight());
    scaleMenu->setBounds((getWidth()/2)-50, 73, 100, 15);
    midiNoteLabel->setBounds((getWidth()/2)-25, getHeight()-70, 50, 20);
}


void GuiCirclePianoForScales::paint (Graphics& g)
{
	g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 0, 800, 800);
	
	for (int i = 0; i <= 47; i++) 
		keys[i]->setBounds(0.0f, 0.0f, getWidth(), getHeight());
	
	for (int i = 0; i <= 4; i++) 
		buttons[i]->setBounds(0.0f, 0.0f, getWidth(), getHeight());
}

void GuiCirclePianoForScales::buttonClicked(Button *button)
{
	for (int i = 0; i <=4; i++)
    {
        //octave/transpose buttons
        if (button == buttons[i])
		{
            switch (i)
            {
                case 0:
                    transposeValue = 0;
                    break;
                case 1:
                    transposeValue = 1;
                    break;
                case 2:
                    transposeValue = 3;
                    break;
                case 3:
                    transposeValue = 5;
                    break;
                case 4:
                    transposeValue = 6;
                    break;
                default:
                    transposeValue = 3;
                    break;
            }

			//transposeValue = i + 1;
            setScale();
		}
	}
	
    
	for (int i = 0; i <=47; i++)
    {
        //piano keys
		if (button == keys[i])
		{
            keyValue = i;
            setScale();
		}
	}
}

void GuiCirclePianoForScales::comboBoxChanged (ComboBox *comboBox)
{
    if (comboBox == scaleMenu)
    {
        setScale();
    }
}


void GuiCirclePianoForScales::setScale()
{
    //This method is used to set the pads MIDI note numbers,  
    //as well as correctly display the scale on the circular piano
    
    //get correct root note from transposeValue and keyValue 
    rootNote = (transposeValue * 12) + keyValue;
    
    //update note label
    setNoteLabelText(rootNote);
    
    //get the selected scale and store it into 'selectedScale' array
    int selectedScale[8];
    bool doNothing = false;
    
    for (int i = 0; i <=7; i++)
    {
        switch (scaleMenu->getSelectedId()) 
        {
            case 1:
                selectedScale[i] = Scales::chromatic[i];
                break;
            case 2:
                selectedScale[i] = Scales::major[i];
                break;
            case 3:
                selectedScale[i] = Scales::majorPentatonic[i];
                break;
            case 4:
                selectedScale[i] = Scales::minor[i];
                break;
            case 5:
                selectedScale[i] = Scales::minorPentatonic[i];
                break;
            case 6:
                selectedScale[i] = Scales::melodicMinor[i];
                break;
            case 7:
                selectedScale[i] = Scales::harmonicMinor[i];
                break;
            case 8:
                selectedScale[i] = Scales::blues[i];
                break;
            case 9:
                selectedScale[i] = Scales::rockNRoll[i];
                break;
            case 10:
                selectedScale[i] = Scales::hungarianGypsy[i];
                break;
            case 11:
                selectedScale[i] = Scales::ragaMadhuri[i];
                break;
                selectedScale[i] = Scales::chromatic[i];
            default:
                doNothing = true;
                break;
        }
    }
    
    //MANUAL METHOD OF MAKING THE KEYS REACT LIKE A RADIO GROUP OF BUTTONS
    //Toggle all keys 'off'
    for (int j = 0; j <= 47; j++)
        keys[j]->setToggleState(false, false);
    
    
    if (doNothing == false)
    {
        //===================SET THE SCALE====================
        
        //get row number. With this equation, row 1 = 1, row 2 = 2 etc... Simples.
        int row = currentlySelectedPad - 99; 
        
        int arrayCount = 0; 
        
        for(int i = (row*8)-8; i <= (row*8)-1; i++) 
        {
            //i = pad number, arrayCount = selectedScale array index
            int note = selectedScale[arrayCount] + rootNote;
            PAD_SETTINGS_i->setMidiNote(note);
            arrayCount++;
        }
        
        //====================================================
        
        
        //===============DISPLAY THE SCALE====================
        //first, need to look at the last note of the scale and see which 
        //'octave region' (transposeValue) the scale needs to be in on the circular piano
        //in order to be fully displayed
        
        int note = selectedScale[7] + rootNote;
        //if note <= 47, need to display using '-36' octave region
        //if note 48-59, need to display using '-24' octave region
        //if note 60-83, need to display using '0' octave region
        //if note 84-107, need to display using '+24' octave region
        //if note >= 108, need to display using '+36' octave region
        
        if (note <= 47)
            transposeValue = 0; // '-36' region
        else if (note >= 48 && note <= 59)
            transposeValue = 1; // '-24' region
        else if (note >= 60 && note <= 83)
            transposeValue = 3; // '0' region
        else if (note >= 84 && note <= 107)
            transposeValue = 5; // '+24' region
        else if (note >= 108)
            transposeValue = 6; // '+36' region
        
        
        if (transposeValue == 0)
        {
            buttons[0]->setToggleState(true, false);
            //keyValue = rootNote-0; 
            
            for (int i = 0; i <=7; i++)
            {
                int note = selectedScale[i] + rootNote;
                int notePianoKey = note-0;
                keys[notePianoKey]->setToggleState(true, false);
            }
            
        }
        
        if (transposeValue == 1)
        {
            buttons[1]->setToggleState(true, false);
            //keyValue = rootNote-12; 
            
            for (int i = 0; i <=7; i++)
            {
                int note = selectedScale[i] + rootNote;
                int notePianoKey = note-12;
                keys[notePianoKey]->setToggleState(true, false);
            }
            
        }
    
        else if (transposeValue == 3)
        {
            buttons[2]->setToggleState(true, false);
            //keyValue = rootNote-36;
            
            for (int i = 0; i <=7; i++)
            {
                int note = selectedScale[i] + rootNote;
                int notePianoKey = note-36;
                keys[notePianoKey]->setToggleState(true, false);
            }
        }
        
        else if (transposeValue == 5)
        {
            buttons[3]->setToggleState(true, false);
            //keyValue = rootNote-60;
            
            for (int i = 0; i <=7; i++)
            {
                int note = selectedScale[i] + rootNote;
                int notePianoKey = note-60;
                keys[notePianoKey]->setToggleState(true, false);
            }
        }
        
        else if (transposeValue == 6)
        {
            buttons[4]->setToggleState(true, false);
            //keyValue = rootNote-72;
            
            for (int i = 0; i <=7; i++)
            {
                int note = selectedScale[i] + rootNote;
                int notePianoKey = note-72;
                
                //check any notes of the scale aren't going out of the range
                //of the piano
                if (notePianoKey <= 47)
                    keys[notePianoKey]->setToggleState(true, false);
            }
            
        }
        

    }
    if (doNothing == true)
    {
        keys[keyValue]->setToggleState(true, false);
    }

}


bool GuiCirclePianoForScales::hitTest (int x, int y)
{
	return hitPath.contains(x, y);
}


//called when a pad is selected from the pad layout
void GuiCirclePianoForScales::setCurrentlySelectedPad (int padNumber)
{
    currentlySelectedPad = padNumber;
}


//called when a pad with midi mode on it needs to be displayed
void GuiCirclePianoForScales::updateDisplay()
{
    noteDisplayType = StoredSettings::getInstance()->midiNoteDisplayType;
    
    for (int j = 0; j <= 47; j++)
        keys[j]->setToggleState(false, false);
    
    scaleMenu->setSelectedId(100, true);
    buttons[2]->setToggleState(true, false);
    keys[24]->setToggleState(true, false);
    setNoteLabelText(60);
    transposeValue = 3;
    keyValue = 24;
	rootNote = 60;
}

void GuiCirclePianoForScales::mouseDown (const MouseEvent &e)
{
    
    if (midiNoteLabel->isMouseOver(true))
    {
        //right click to bring up the popup menu to allow single presets to be loaded and saved to disk
        if (e.mods.isPopupMenu() == true)
        {
            
            PopupMenu menu;
            
            if (noteDisplayType == 1)
                menu.addItem(1, "Display MIDI Numbers");
            else
                menu.addItem(2, "Display MIDI Notes");
            
            const int result = menu.show();
            
            if (result == 0)
            {
                // user dismissed the menu without picking anything
            } 
            else if (result == 1)
            {
                noteDisplayType = 0; // note number
                setNoteLabelText(rootNote);
                StoredSettings::getInstance()->midiNoteDisplayType = 1;
            }
            else if (result == 2)
            {
                noteDisplayType = 1; // note name
                setNoteLabelText(rootNote);
                StoredSettings::getInstance()->midiNoteDisplayType = 2;
            }
        }
    }
}



void GuiCirclePianoForScales::mouseEnter (const MouseEvent &e)
{
    for (int i = 0; i <= 47; i++)
    {
        //===display note number when hovering mouse over a piano key===
        if (keys[i]->isMouseOver(true))
        {
            int newNote = (transposeValue * 12) + i;
            setNoteLabelText(newNote);
            midiNoteLabel->setColour(Label::textColourId, Colours::orange);
        }
    }
}


void GuiCirclePianoForScales::mouseExit (const MouseEvent &e)
{
    //=====set key label text to currently selected note=====
    setNoteLabelText(rootNote);
    midiNoteLabel->setColour(Label::textColourId, Colours::lightgrey);
}


void GuiCirclePianoForScales::setNoteLabelText (int noteNumber)
{
    if (StoredSettings::getInstance()->midiNoteDisplayType == 1)
        midiNoteLabel->setText(String(noteNumber), false);
    
    else if (StoredSettings::getInstance()->midiNoteDisplayType == 2)
        midiNoteLabel->setText(MidiMessage::getMidiNoteName(noteNumber, true, true, 3), false);
}


