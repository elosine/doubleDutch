(
s.waitForBoot{ fork{
	~freqBus = Bus.control(s);
	~ampBus = Bus.control(s);

	~oCtr = 0;
	~ropes = [0,1,2,3,4,5,6,7,8];
	~of = NetAddr("127.0.0.1", 12321);
	~paletteArrayA = ["dekooning", "kandinsky", "johns", "klee"];
	~paletteArrayB = Array.newFrom(~paletteArrayA.scramble);
	~paletteNum = 0;
	~numOnsetsClrChgL = 80;
	~numOnsetsClrChgH = 150; // ~ca 200 per minute
	~clrChgOnsets = rrand(~numOnsetsClrChgL, ~numOnsetsClrChgH).round;
	~chgClr = true;

	~onsetR = OSCFunc( { |msg|
		~oCtr = ~oCtr+1;
		~oCtr.postln;

		//Change Color Palette
		if( ~chgClr, {
		if( (~oCtr%~clrChgOnsets) == 0,
			{
				~chgClrR.value(~paletteArrayB[~paletteNum]);
				~paletteNum = (~paletteNum+1)%~paletteArrayB.size.asInteger;
				~clrChgOnsets = rrand(~numOnsetsClrChgL, ~numOnsetsClrChgH).round;
		});
		});

	}, "/onset" );

	SynthDef(\onset, {
		arg thresh = 0.3, sibs=0;
		var sound, fft, analysis;
		sound = SoundIn.ar(sibs);
		fft = FFT(LocalBuf(512),sound);
		analysis = Onsets.kr(fft,threshold:thresh);
		SendReply.kr( analysis, "/onset" );
	}).add;

	SynthDef(\detect, {
		arg freqOut, ampOut, sibs=0;
		var amp, freq, hasFreq, in;
		in = SoundIn.ar(sibs);
		amp = Amplitude.kr(in);
		#freq, hasFreq = Pitch.kr(in,median:7);
		freq.cpsmidi.poll;
		Out.kr(freqOut, freq.cpsmidi);
		Out.kr(ampOut, amp.ampdb);
	}).add;

	OSCdef(\getFreqAmp, {
		arg msg;
		var freq, amp;
		freq = ~freqBus.getSynchronous.asFloat;
		amp = ~ampBus.getSynchronous.asFloat;
		~of.sendMsg("/freqAmp", freq, amp);
		//freq.postln;
		//msg.postln;
	}, "/getFreqAmp");

	~of = NetAddr("127.0.0.1", 12321);

	0.1.wait;

	a = Synth(\detect, [\freqOut, ~freqBus, \ampOut, ~ampBus, \sibs, 1]);
	b = Synth(\onset, [\thresh, 0.3, \sibs, 1]);


	~chgClrR = {| paletteName| Routine( {
		var rps = Array.newFrom(~ropes.scramble);
		rps.do{ |it ix|
			~of.sendMsg("/chgClr", it, paletteName);
			rrand(0.4, 1.5).wait;
		};
	}).play };

	~clk0 = TempoClock(1);

	~clk0.schedAbs(
		1,
		{|msg|
			var dir = rand(6).floor;
			var wt = rrand(0.5,1.1);
			~of.sendMsg("/catPushDir", dir);
			wt
		}
	);

	~clk0.sched(110, {~of.sendMsg("/chgRopeMode", 1); nil});
	~clk0.sched(160, {~of.sendMsg("/chgRopeMode", 2); nil});
	~clk0.sched(210, {~of.sendMsg("/chgRopeMode", 3); nil});
	~clk0.sched(300, {~chgClr = false; ~of.sendMsg("/chgRopeMode", 4); nil});


	{ Out.ar( 0, [SoundIn.ar(1), SoundIn.ar(1)] ) }.play;
}}
)












