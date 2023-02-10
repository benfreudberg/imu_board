pkg load instrument-control
clear
close all

SYNC_SIGNAL = typecast(uint8([0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01]), "uint64");
bytes_per_var = 2;
num_vars = 3;
num_sets = 10;

sets_per_window = 50;
num_points = num_sets*sets_per_window;

figure
mx_data = zeros(1, num_points);
my_data = zeros(1, num_points);
mz_data = zeros(1, num_points);
t = 1:num_points;
mx_plot = plot(t, mx_data);
hold
my_plot = plot(t, my_data);
mz_plot = plot(t, mz_data);

window = 0;

s1 = serial("\\\\.\\COM20", 9600);
srl_flush(s1);

sync_found = false;
while (!sync_found)
  sync_signal = uint8(srl_read(s1,8));
  sync_signal_num = typecast(sync_signal, "uint64");
  if (sync_signal_num == SYNC_SIGNAL)
    sync_found = true;
  else
    srl_flush(s1);
  endif
endwhile


while true
  data_raw = uint8(srl_read(s1,bytes_per_var*num_vars*num_sets));
  data = typecast(data_raw, "int16");
  for i = 1:num_sets
    mx(i) = data(num_vars*(i-1) + 1);
    my(i) = data(num_vars*(i-1) + 2);
    mz(i) = data(num_vars*(i-1) + 3);
  endfor
  
  mx_data(window*num_sets + 1 : (window + 1)*num_sets) = mx;
  my_data(window*num_sets + 1 : (window + 1)*num_sets) = my;
  mz_data(window*num_sets + 1 : (window + 1)*num_sets) = mz;
  set(mx_plot, 'ydata', mx_data);
  set(my_plot, 'ydata', my_data);
  set(mz_plot, 'ydata', mz_data);
  
  
  
  drawnow
  
  sync_signal = uint8(srl_read(s1,8));
  window++;
  window = mod(window,sets_per_window);
endwhile
srl_close(s1);