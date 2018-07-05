clc;clear;close all;
%%
image_indexes = [3, 4, 7, 8, 13, 25, 31, 34, 35, 38, 39, 50];
%%
for i = 1:length(image_indexes)
    I = imread([num2str(image_indexes(i)) '.bmp']);
    homo_I = test_tontai(I);
    homo_I = uint8((homo_I - min(homo_I(:))) / (max(homo_I(:)) - min(homo_I(:))) * 255);
    imwrite(homo_I, [num2str(image_indexes(i)) '.tif'])
end