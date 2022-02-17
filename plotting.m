%% 100 trajektorii cez transformaciu
data100_transf = readmatrix('data100_transf.csv');

% Zistenie poctu trajektorii
[count, ~] = size(data100_transf);

% Vizualizacia
figure('Name', '100 trajectories', 'NumberTitle', 'off');
title('100 trajectories of Ito process by transform')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
hold on
for n = 2:count
	plot(data100_transf(1, :), data100_transf(n, :), ...
		'-', 'LineWidth', 0.5);
end
plot(data100_transf(1, 1), data100_transf(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% 1000 trajektorii cez transformaciu
data1000_transf = readmatrix('data1000_transf.csv');
% Zistenie poctu trajektorii
[count, ~] = size(data1000_transf);

% Vizualizacia
figure('Name', '1000 trajectories', 'NumberTitle', 'off');
title('1000 trajectories of Ito process by transform')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
hold on
for n = 2:count
	plot(data1000_transf(1, :), data1000_transf(n, :), ...
		'-', 'LineWidth', 1);
end
plot(data1000_transf(1, 1), data1000_transf(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off

%% Trajektorie z DEFINICIE
%% 100 trajektorii
%% Zistenie poctu trajektorii
data100_def = readmatrix('data100_def.csv');
[count, ~] = size(data100_def);

% Vizualizacia
figure('Name', '100 trajectories', 'NumberTitle', 'off');
title('100 trajectories of Ito process by definition')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
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

% Vizualizacia
figure('Name', '1000 trajectories definition', 'NumberTitle', 'off');
title('1000 trajectories of Ito process by definition')
xlabel('Time', 'FontSize', 20)
ylabel('Angle θ', 'FontSize', 20)
hold on
for n = 2:count
	plot(data1000_def(1, 1:end-1), data1000_def(n, 1:end-1), ...
		'-', 'LineWidth', 0.5);
end
plot(data1000_def(1, 1), data1000_def(2, 1), '.', 'Color', 'red', 'MarkerSize', 25);
hold off
