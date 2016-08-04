function plot_spectrogram(file, start, finish)
% start, finishは波形を読み込む開始位置と終了位置を秒で与える
% 秒からサンプル位置への変換
start = start * 44100;
finish = finish * 44100;
samples = [start+1, finish];
[y, fs] = audioread(file, samples);
N = 2048;

subplot(2,1,1);
plot(y);
xlabel('samples');
ylabel('amplitude');

subplot(2,1,2);
spectrogram(y, hamming(N), 0, N);
xlabel('samples');
ylabel('frequency');