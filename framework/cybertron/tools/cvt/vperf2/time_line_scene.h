/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include "time_line_row.h"

class TimeLineTable;
class TimeLineAxis;
class TimeBlockItem;
class TimeLineLegend;

class TimeLineScene : public QGraphicsScene {
 public:
  explicit TimeLineScene(QObject* parent = nullptr);
  ~TimeLineScene() {}

  TimeLineRow* addOneTimeLineByProcessorIndex(int processorIndex) {
    if (processorIndex < 0) return nullptr;
    return addOneTimeLineByRowIndex(processorIndex, _processorTable);
  }

  TimeBlockItem* addTimeBlock(TimeLineRow* row, const QString& blockName,
                              int64_t& start, int64_t& end,
                              bool isEventBlock = false);

  TimeBlockItem* addTimeBlock(int processorIndex, const QString& blockName,
                              int64_t& start, int64_t& end) {
    TimeLineRow* r = addOneTimeLineByProcessorIndex(processorIndex);
    if (r == nullptr) return nullptr;
    if (!r->hasRowHeaderText()) {
      r->setRowHeaderText(tr("%1").arg(processorIndex));
      r->adjustHeaderTextPos(_headerWidth, _rowHeight);
    }
    return addTimeBlock(r, blockName, start, end);
  }

  int64_t timeBegin(void) const { return _timeBegin; }
  int64_t timeEnd(void) const { return _timeEnd; }

  void setTimeBegin(int64_t b) { _timeBegin = b; }
  void setTimeEnd(int64_t t);

  int timeLineLength(void) const {
    return (_timeEnd - _timeBegin + timeRatio() - 1) / timeRatio();
  }

  int timeRatio(void) const { return _timeRatio; }

  int axisScale(void) const { return _axisScale; }
  int axisSparsity(void) const { return _axisSparsity; }
  int rowHeaderWidth(void) const { return _headerWidth; }
  int rowHeight(void) const { return _rowHeight; }

  int sceneWidth(void) const { return sceneRect().width(); }

  int timeLength2Width(int l) const { return l * _axisScale + _headerWidth; }

  void setTimeRatio(int tr);
  void setSceneScale(int s);
  void setSceneSparsity(int s);
  void setRowHeaderWidth(int w);
  void setRowHeight(int h);

  int processorCount(void) const;

  void keepStill(qreal v);
  void sceneDoubleClicked(QGraphicsItem*, QPointF& scenePos);

 private:
  TimeLineRow* addOneTimeLineByRowIndex(int processorIndex, TimeLineTable*);
  void showProcessorTasks(QPointF& scenePos);
  void showTimeBlockInfo(TimeBlockItem* item);
  void createTimeGrid(void);
  void destroyTimeGrid(void);
  void adjustTimeGrid(void);

  int _axisScale;
  int _axisSparsity;
  int _headerWidth;
  int _rowHeight;
  int _rowOffset;

  int _currentProcessorIndex;
  int _timeRatio;
  int64_t _timeBegin;
  int64_t _timeEnd;

  TimeLineAxis* _newAxis;
  TimeLineTable* _processorTable;
  TimeLineTable* _taskTable;
  TimeLineLegend* _taskLegend;
  QGraphicsTextItem* _taskProcessorName;
  QGraphicsItemGroup* _timeGrid;

  friend class TimeLineWidget;
};

#endif  // TIMELINESCENE_H