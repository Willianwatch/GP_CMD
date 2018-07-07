clc;clear;close all;
%%
image_indexes = 0:50;
%%
for i = 1:length(image_indexes)
    I = imread([num2str(image_indexes(i)) '.bmp']);
    if (length(size(I)) == 3)
        I = rgb2gray(I);
    end
    homo_I = test_tontai(I);
    homo_I = uint8((homo_I - min(homo_I(:))) / (max(homo_I(:)) - min(homo_I(:))) * 255);
    imwrite(homo_I, [num2str(image_indexes(i)) '.tif'])
end