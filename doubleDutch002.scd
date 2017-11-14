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
	~numOnsetsClrChgL = 325;
	~numOnsetsClrChgH = 425; // ~ca 400 per minute
	~clrChgOnsets = rrand(~numOnsetsClrChgL, ~numOnsetsClrChgH).round;
	~chgClr = true;

	~onsetR = OSCFunc( { |msg|
		~oCtr = ~oCtr+1; ~oCtr.postln;

		//Change Color Palette
		if( ~chgClr, {
		if( (~oCtr%~clrChgOnsets) == 0,
			{
				"fdsafdsajfdksalfjdsaklfjdsjfdsklafjdsklafjdsakld".postln;
				~chgClrR.value(~paletteArrayB[~paletteNum]);
				~paletteNum = (~paletteNum+1)%~paletteArrayB.size.asInteger;
				~paletteNum.postln;
				~clrChgOnsets = rrand(~numOnsetsClrChgL, ~numOnsetsClrChgH).round;
		});
		});

	}, "/onset" );

	SynthDef(\onset, {
		arg thresh = 0.3;
		var sound, fft, analysis;
		sound = SoundIn.ar(0);
		fft = FFT(LocalBuf(512),sound);
		analysis = Onsets.kr(fft,threshold:thresh);
		SendReply.kr( analysis, "/onset" );
	}).add;

	SynthDef(\detect, {
		arg freqOut, ampOut;
		var amp, freq, hasFreq, in;
		in = SoundIn.ar(0);
		amp = Amplitude.kr(in);
		#freq, hasFreq = Pitch.kr(in,median:7);
		freq.cpsmidi/*.poll*/;
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

	a = Synth(\detect, [\freqOut, ~freqBus, \ampOut, ~ampBus]);
	b = Synth(\onset, [\thresh, 0.3]);


	~chgClrR = {| paletteName| Routine( {
		var rps = Array.newFrom(~ropes.scramble);
		paletteName.postln;
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
			var wt = rrand(0.2,1);
			~of.sendMsg("/catPushDir", dir);
			wt
		}
	);

	/*~clk0.sched(140, {~of.sendMsg("/chgRopeMode", 1); nil});
	~clk0.sched(260, {~of.sendMsg("/chgRopeMode", 2); nil});
	~clk0.sched(380, {~of.sendMsg("/chgRopeMode", 3); nil});
	~clk0.sched(380, {~chgClr = false; ~of.sendMsg("/chgRopeMode", 4); nil});*/


	~clk0.sched(10, {~of.sendMsg("/chgRopeMode", 1); nil});
	~clk0.sched(20, {~of.sendMsg("/chgRopeMode", 2); nil});
	~clk0.sched(30, {~of.sendMsg("/chgRopeMode", 3); nil});
	~clk0.sched(40, {~chgClr = false; ~of.sendMsg("/chgRopeMode", 4); nil});

}}
)
~of.sendMsg("/chgRopeMode", 1);
~of.sendMsg("/chgRopeMode", 2);











