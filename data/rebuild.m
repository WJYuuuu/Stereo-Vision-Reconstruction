clc;
clear;
% 读取 CSV 文件中的点云数据
pointCloudData = csvread('D:\Desktop\code\QTproject\photo\point_cloud.csv', 1, 0); % 假设 CSV 文件中的数据是以逗号分隔的
% 将第三列置为 0
% pointCloudData(:, 3) = 0;
% 清理异常数据
validIndices = all(isfinite(pointCloudData), 2); % 保留所有有限的数据行
pointCloudData = pointCloudData(validIndices, :); % 保留有效点


% 计算每个维度的四分位距
Q1 = quantile(pointCloudData, 0.25);
Q3 = quantile(pointCloudData, 0.75);
IQR = Q3 - Q1;

% 定义异常值的范围
lowerBound = Q1 - 1.5 * IQR;
upperBound = Q3 + 1.5 * IQR;

% 找到在异常值范围之内的数据
validIndices = true(size(pointCloudData, 1), 1); % 初始化为全为真
for dim = 1:size(pointCloudData, 2)
    validIndices = validIndices & (pointCloudData(:, dim) >= lowerBound(dim)) & (pointCloudData(:, dim) <= upperBound(dim));
end

% 保留有效点
pointCloudData = pointCloudData(validIndices, :);

% 计算点云的中心点
center = mean(pointCloudData); % 计算点云的中心点

xlabel('X');
ylabel('Y');
zlabel('Z');
hold on;
pcshow(pointCloudData);
