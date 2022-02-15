%% Nacitanie dat
data100 = readmatrix('data100.csv');
data1000 = readmatrix('data1000.csv');

%% Zistenie poctu trajektorii
[count, ~] = size(data100);
count = count - 1; % minus prvy riadok, kde je ulozena casova os

%% Vizualizacia
figure('Name', '100 trajectories', 'NumberTitle', 'off');
title('100 trajectories of Ito process')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
yticks(-45:15:80)
ytickformat('%.0f°')
hold on
for n = 2:count
	plot(data100(1, :), data100(n, :), ...
		'-', 'LineWidth', 0.5);
end
plot(data100(1, 1), data100(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% Zistenie poctu trajektorii
[count, ~] = size(data1000);
count = count - 1; % minus prvy riadok, kde je ulozena casova os

%% Vizualizacia
figure('Name', '1000 trajectories', 'NumberTitle', 'off');
title('1000 trajectories of Ito process')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
ytickformat('%.0f°')
yticks(-45:15:80)
hold on
for n = 2:count
	plot(data1000(1, :), data1000(n, :), ...
		'-', 'LineWidth', 1);
end
plot(data1000(1, 1), data1000(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% Trajektorie z DEFINICIE
%% 100 trajektorii
%% Zistenie poctu trajektorii
data100_def = readmatrix('data100_def.csv');
[count, ~] = size(data100_def);
count = count - 1; % minus prvy riadok, kde je ulozena casova os

%% Vizualizacia
figure('Name', '100 trajectories', 'NumberTitle', 'off');
title('100 trajectories of Ito process')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
% ytickformat('%.0f°')
hold on
for n = 2:count
	plot(data100_def(1, :), data100_def(n, :), ...
		'-', 'LineWidth', 0.5);
end
plot(data100_def(1, 1), data100_def(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% 100 trajektorii cez definiciu
%% Zistenie poctu trajektorii
data100_def = readmatrix('data100_def.csv');
[count, ~] = size(data100_def);
count = count - 1; % minus prvy riadok, kde je ulozena casova os

%% Vizualizacia
figure('Name', '100 trajectories', 'NumberTitle', 'off');
title('100 trajectories of Ito process')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
% ytickformat('%.0f°')
hold on
for n = 2:count
	plot(data100_def(1, :), data100_def(n, :), ...
		'-', 'LineWidth', 0.5);
end
plot(data100_def(1, 1), data100_def(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% 1000 trajektorii cez definiciu
%% Zistenie poctu trajektorii
data1000_def = readmatrix('data1000_def.csv');
[count, ~] = size(data1000_def);
count = count - 1; % minus prvy riadok, kde je ulozena casova os

%% Vizualizacia
figure('Name', '1000 trajectories', 'NumberTitle', 'off');
title('1000 trajectories of Ito process')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
% ytickformat('%.0f°')
hold on
for n = 2:count
	plot(data1000_def(1, 1:end-1), data1000_def(n, 1:end-1), ...
		'-', 'LineWidth', 0.5);
end
plot(data1000_def(1, 1), data1000_def(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off
