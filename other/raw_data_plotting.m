pkg load instrument-control
clear
close all

SYNC_SIGNAL = typecast(uint8([0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01]), "uint64");
bytes_per_var = 2;
num_vars = 9;
num_sets = 100;

sets_per_window = 10;
num_points = num_sets*sets_per_window;

figure
ax_data = zeros(1, num_points);
ay_data = zeros(1, num_points);
az_data = zeros(1, num_points);
t = 1:num_points;
ax_plot = plot(t, ax_data);
hold
ay_plot = plot(t, ay_data);
az_plot = plot(t, az_data);

figure
gx_data = zeros(1, num_points);
gy_data = zeros(1, num_points);
gz_data = zeros(1, num_points);
gx_plot = plot(t, gx_data);
hold
gy_plot = plot(t, gy_data);
gz_plot = plot(t, gz_data);

figure
mx_data = zeros(1, num_points);
my_data = zeros(1, num_points);
mz_data = zeros(1, num_points);
mx_plot = plot(t, mx_data);
hold
my_plot = plot(t, my_data);
mz_plot = plot(t, mz_data);


window = 0;

SYNC_SIGNAL = typecast(uint8([0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01]), "uint64");
bytes_per_var = 2;
num_vars = 9;
num_sets = 100;

s1 = serial("com3", 460800);
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
    ax(i) = data(9*(i-1) + 1);
    ay(i) = data(9*(i-1) + 2);
    az(i) = data(9*(i-1) + 3);
    
    gx(i) = data(9*(i-1) + 4);
    gy(i) = data(9*(i-1) + 5);
    gz(i) = data(9*(i-1) + 6);
    
    mx(i) = data(9*(i-1) + 7);
    my(i) = data(9*(i-1) + 8);
    mz(i) = data(9*(i-1) + 9);
  endfor
  ax_data(window*num_sets + 1 : (window + 1)*num_sets) = ax;
  ay_data(window*num_sets + 1 : (window + 1)*num_sets) = ay;
  az_data(window*num_sets + 1 : (window + 1)*num_sets) = az;
  set(ax_plot, 'ydata', ax_data);
  set(ay_plot, 'ydata', ay_data);
  set(az_plot, 'ydata', az_data);
  
  gx_data(window*num_sets + 1 : (window + 1)*num_sets) = gx;
  gy_data(window*num_sets + 1 : (window + 1)*num_sets) = gy;
  gz_data(window*num_sets + 1 : (window + 1)*num_sets) = gz;
  set(gx_plot, 'ydata', gx_data);
  set(gy_plot, 'ydata', gy_data);
  set(gz_plot, 'ydata', gz_data);
  
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