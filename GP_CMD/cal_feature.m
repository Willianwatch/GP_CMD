close all;clear;clc;
%%
li = [3, 4, 8, 13, 25, 34, 35, 43, 50];
fearture = zeros(length(li), 36);
%%
for i=1:length(li)
    index = li(i);
    image1 = imread(['TrainingDataSet/TrainingImages/' num2str(index) '.bmp']);
    if length(size(image1))==3
        image1 = rgb2gray(image1);
    end
    
    image2 = imread(['TrainingDataSet/TargetImages/' num2str(index) '.bmp']);
    
    fearture(i, :) = FeatureExtraction(image1, image2, 1)';
end