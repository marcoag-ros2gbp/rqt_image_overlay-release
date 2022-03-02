// Copyright 2021 Kenji Brameld
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IMAGE_MANAGER_HPP_
#define IMAGE_MANAGER_HPP_

#include <QAbstractListModel>
#include <QImage>
#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include "image_transport/subscriber.hpp"
#include "image_topic.hpp"
#include "msg_storage.hpp"
#include "overlay_time_info.hpp"

namespace rclcpp {class Node;}

namespace rqt_image_overlay
{

class ImageManager : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit ImageManager(const std::shared_ptr<rclcpp::Node> & node);

  bool imageAvailable() const;

  // throws StorageEmptyException if storage is empty
  std::tuple<std::shared_ptr<QImage>, rclcpp::Time> getClosestImageAndHeaderTime(
    const rclcpp::Time & targetTimeReceived) const;

  std::optional<ImageTopic> getImageTopic(unsigned index);
  void addImageTopicExplicitly(ImageTopic imageTopic);

protected:
  int rowCount(const QModelIndex & parent = QModelIndex()) const override;
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

public slots:
  void onTopicChanged(int index);
  void updateImageTopicList();

private:
  void callbackImage(const sensor_msgs::msg::Image::ConstSharedPtr & msg);

  image_transport::Subscriber subscriber;
  const std::shared_ptr<rclcpp::Node> & node;
  MsgStorage<sensor_msgs::msg::Image::ConstSharedPtr> msgStorage;
  rclcpp::Clock systemClock{RCL_SYSTEM_TIME};

  std::vector<ImageTopic> imageTopics;
};

}  // namespace rqt_image_overlay

#endif  // IMAGE_MANAGER_HPP_
