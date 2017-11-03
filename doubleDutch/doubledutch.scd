(
s.waitForBoot{ fork{
	~freqBus = Bus.control(s);
	~ampBus = Bus.control(s);

	~oCtr = 0;
	~onsetR = OSCFunc( { |msg| ~oCtr = ~oCtr+1; ~oCtr.postln }, "/onset" );

	SynthDef(\onset, {
		arg thresh = 0.3;
		var sound, fft, analysis;
		sound = SoundIn.ar(0);
		fft = FFT(LocalBuf(512),sound);
		analysis = Onsets.kr(fft,threshold:thresh);
		SendReply.kr( analysis, "/onset" );
		Out.ar(0,sound);
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
}}
)
b.free;
a.free;
(
SynthDef(\onset, {
	var sound, fft, analysis;
	sound = SoundIn.ar(0);
	fft = FFT(LocalBuf(512),sound);
	analysis = Onsets.kr(fft,threshold:0.1, odftype: \complex);
	SendReply.kr( analysis, "/onset" );
	Out.ar(0,sound);
}).add;
)

~ctr = 0;
o = OSCFunc( { |msg| ~ctr = ~ctr+1; ~ctr.postln }, "/onset" );





