#include "framework.h"


int string_before(string & strBefore, string str, char ch, int iStart = 0)
{
   strsize iFind = str.find_ci(ch, iStart);
   if (iFind < 0)
   {
      return iFind;
   }
   strBefore = str.Mid(iStart, iFind - iStart);
   return iFind;
}

bool isResetCommand(string data)
{
   if (data.contains_ci("M30"))
   {
      return true;
   }
   int iFind = -1;
   while ((iFind = data.find_ci("M0", iFind + 2)) >= 0)
   {
      if (data.find('2', iFind + 2) >= iFind + 2)
      {

         return true;

      }
   }
   return false;
}

bool dataIsEnd(string data)
{
   stringa stra;

   stra.add("ok");
   stra.add("error");

   for (auto & str : stra)
   {
      if (data.contains_ci(str)) return true;
   }

   return false;
}

bool dataIsFloating(string data)
{
   stringa stra;

   stra.add("Reset to continue");
   stra.add("'$H'|'$X' to unlock");
   stra.add("ALARM: Soft limit");
   stra.add("ALARM: Hard limit");
   stra.add("Check Door");

   for (auto & str : stra)
   {
      if (data.contains_ci(str)) return true;
   }

   return false;
}
bool eat_space(string & str)
{
   if (str.get_length() <= 0)
   {
      return false;
   }
   if (!isspace_dup(str[0]))
   {
      return false;
   }
   str = str.Mid(1);
   return true;
}
bool eat_digit(string & str)
{
   if (str.get_length() <= 0)
   {
      return false;
   }
   if (!isdigit_dup(str[0]))
   {
      return false;
   }
   str = str.Mid(1);
   return true;
}


bool dataIsReset(string data)
{

   if (!::str::begins_eat_ci(data, "GRBL") && !::str::begins_eat_ci(data, "GCARVIN"))
   {

      return false;

   }

   if (!eat_space(data))
   {

      return false;

   }

   if (!eat_digit(data))
   {

      return false;

   }

   if (!::str::begins_eat(data, "."))
   {

      return false;

   }

   if (!eat_digit(data))
   {

      return false;

   }

   if (data.get_length() <= 0)
   {

      return false;

   }

   return true;

}


namespace grbl_merge_tool
{




   control::control(::aura::application * papp) :
      ::object(papp),
      m_mutexFree(papp),
      m_evFree(papp),
      m_mutexSerial(papp)
   {

      m_pcommandaFile = NULL;

      defer_create_mutex();
//      m_iLastGrblStatus = -1;
      m_straStatus << "Unknown" << "Idle" << "Alarm" << "Run" << "Home" << "Hold" << "Queue" << "Check" << "Door";
//      m_bHoming = false;
//      m_bStatusReceived = false;
      m_bResetCompleted = true;
//      m_bRun = false;
      m_evFree.ResetEvent();
//      m_bWriting = false;
      m_iMaxBufferSize = 127;
      //m_bNoTabs = true;
//      m_bReseting = false;
//      m_iFileCommandIndex = 0;


   }


   control::~control()
   {

   }


   void control::assert_valid() const
   {

      ::object::assert_valid();

   }


   void control::dump(dump_context & dumpcontext) const
   {

      ::object::dump(dumpcontext);

   }



   bool control::open_serial_port(string strPort, int iBaud)
   {

      synch_lock sl(mutex());

      if (strPort.is_empty())
      {

         return false;

      }

      if (iBaud <= 0)
      {

         return false;

      }

      if (m_pserial.is_set() && m_pserial->getPort() == strPort && m_pserial->getBaudrate() == iBaud)
      {

         return true;

      }

      sl.unlock();

      if (m_pthreadRead.is_set())
      {

         ::multithreading::post_quit_and_wait(m_pthreadRead, seconds(15));

      }

      sl.lock();

      try
      {

         m_pserial.release();

         m_pserial = canew(serial::Serial(get_context_application(), strPort, iBaud, ::serial::Timeout::simpleTimeout(500)));

      }
      catch (invalid_argument_exception *pe)
      {

         //Application.simple_message_box(m_ppaneview->GetTopLevelFrame(), m_strPort + "> Failed. " + +pe->get_message());

      }
      catch (serial::SerialException *pe)
      {

         //Application.simple_message_box(m_ppaneview->GetTopLevelFrame(), m_strPort + " > Failed. " + pe->what());

      }
      catch (serial::IOException * pe)
      {

         string strResult = "Failed. " + string(pe->what());

         string strExtra;


         //Application.simple_message_box(m_ppaneview->GetTopLevelFrame(), m_strPort + "> IO Error " + strResult + " " + strExtra);

      }
      catch (...)
      {

         //Application.simple_message_box(m_ppaneview->GetTopLevelFrame(), m_strPort + "> Failed. Could not open connection.");

      }

      if (m_pserial.is_null())
      {

         Application.m_strStatus = "Serial not connected";

         return false;

      }

      m_pthreadRead = fork([this]()
      {

         read();

      });

      return true;

   }





   void pane_view::on_create_impact(::user::impact_creator_data * pdata)
   {

      switch (pdata->m_id)
      {
      case pane_main:

         create_view < main_view >(pdata);

         break;
      }

      ::userex::pane_tab_view::on_create_impact(pdata);

   }


   void control::read()
   {

      try
      {

         string data;

         string response;

         DWORD dwLastStatus = get_tick_count();

         if (m_pserial.is_set())
         {

            Application.m_strStatus = "Port connected";

         }

         while (::thread_get_run())
         {

            {

               synch_lock sl(&m_mutexSerial);

               if (m_pserial.is_null())
               {

                  Application.m_strStatus = "Port not connected";

                  break;

               }

               data = m_pserial->readline();

            }

            if (data.is_empty())
            {

               Sleep(10);

               if (m_pserial.is_set() && m_bResetCompleted && m_bStatusReceived)
               {

                  if(get_tick_count() - dwLastStatus > 250)
                  {

                     synch_lock sl(&m_mutexSerial);

                     u8 uch = (char) '?';

                     m_pserial->write(&uch, 1);

                     m_bStatusReceived = false;

                     dwLastStatus = get_tick_count();

                  }

               }

               continue;

            }

            if (m_bResetting)
            {

               if (!dataIsReset(data))
               {

                  continue;

               }
               else
               {

                  m_bResetting = false;

               }

            }

            // Status response
            if (data[0] == '<')
            {

               int status = -1;

               m_bStatusReceived = true;

               dwLastStatus = get_tick_count();
               // Update machine coordinates

               strsize iFind = data.find("MPos:");
               string strX;
               string strY;
               string strZ;
               //QRegExp mpx("MPos:([^,]*),([^,]*),([^,^>]*)");
               if (iFind >= 0
                     && (iFind = string_before(strX, data, ',', iFind + 5)) > 0
                     && (iFind = string_before(strY, data, ',', iFind + 1)) > 0
                     && (iFind = string_before(strZ, data, ',', iFind + 1)) > 0)
               {
                  Application.m_straCoord[3] = strX;
                  Application.m_straCoord[4] = strY;
                  Application.m_straCoord[5] = strZ;
               }

               // Status
               // QRegExp stx("<([^,^>]*)");
               iFind = data.find("<");
               string strStatus;
               //QRegExp mpx("MPos:([^,]*),([^,]*),([^,^>]*)");
               if (iFind >= 0 &&
                     (iFind = string_before(strStatus, data, ',', iFind + 1)) > 0)
               {

                  int status = m_straStatus.find_first_ci(strStatus);

                  // Undetermined status
                  if (status == -1) status = 0;

                  // Update status
                  if (status != m_iLastGrblStatus)
                  {
                     //ui->txtStatus->setText(m_statusCaptions[status]);
                     //ui->txtStatus->setStyleSheet(QString("background-color: %1; color: %2;")
                     //                             .arg(m_statusBackColors[status]).arg(m_statusForeColors[status]));
                  }

                  // Update controls
                  //               ui->cmdRestoreOrigin->setEnabled(status == IDLE);
                  //               ui->cmdSafePosition->setEnabled(status == IDLE);
                  //               ui->cmdZeroXY->setEnabled(status == IDLE);
                  //               ui->cmdZeroZ->setEnabled(status == IDLE);
                  //               ui->chkTestMode->setEnabled(status != RUN && !m_processingFile);
                  //               ui->chkTestMode->setChecked(status == CHECK);
                  //               ui->cmdFilePause->setChecked(status == HOLD || status == QUEUE);
                  //#ifdef WINDOWS
                  //               if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7)
                  //               {
                  //                  if (m_taskBarProgress) m_taskBarProgress->setPaused(status == HOLD || status == QUEUE);
                  //               }
                  //#endif
                  //
                  //               // Update "elapsed time" timer
                  //               if (m_processingFile)
                  //               {
                  //                  QTime time(0, 0, 0);
                  //                  int elapsed = m_startTime.elapsed();
                  //                  ui->glwVisualizer->setSpendTime(time.addMSecs(elapsed));
                  //               }
                  //
                  //               // Test for job complete
                  //               if (m_processingFile && m_transferCompleted &&
                  //                     ((status == IDLE && m_lastGrblStatus == RUN) || status == CHECK))
                  //               {
                  //                  qDebug() << "job completed:" << m_fileCommandIndex << m_currentModel->rowCount() - 1;
                  //
                  //                  // Shadow last segment
                  //                  GcodeViewParse *parser = m_currentDrawer->viewParser();
                  //                  QList<LineSegment*> list = parser->getLineSegmentList();
                  //                  if (m_lastDrawnLineIndex < list.count())
                  //                  {
                  //                     list[m_lastDrawnLineIndex]->setDrawn(true);
                  //                     m_currentDrawer->update(QList<int>() << m_lastDrawnLineIndex);
                  //                  }
                  //
                  //                  // Update state
                  //                  m_processingFile = false;
                  //                  m_fileProcessedCommandIndex = 0;
                  //                  m_lastDrawnLineIndex = 0;
                  //                  m_storedParserStatus.clear();
                  //
                  //                  updateControlsState();
                  //
                  //                  qApp->beep();
                  //
                  //                  m_timerStateQuery.stop();
                  //                  m_timerConnection.stop();
                  //
                  //                  QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                  //                                           .arg(ui->glwVisualizer->spendTime().toString("hh:mm:ss")));
                  //
                  //                  m_timerStateQuery.setInterval(m_settings->queryStateTime());
                  //                  m_timerConnection.start();
                  //                  m_timerStateQuery.start();
                  //               }

                  //// Store status
                  //if (status != m_lastGrblStatus) m_lastGrblStatus = status;

                  //// Abort
                  //static double x = sNan;
                  //static double y = sNan;
                  //static double z = sNan;

                  //if (m_aborting)
                  //{
                  //   switch (status)
                  //   {
                  //   case IDLE: // Idle
                  //      if (!m_processingFile && m_resetCompleted)
                  //      {
                  //         m_aborting = false;
                  //         restoreOffsets();
                  //         restoreParserState();
                  //         return;
                  //      }
                  //      break;
                  //   case HOLD: // Hold
                  //   case QUEUE:
                  //      if (!m_reseting && compareCoordinates(x, y, z))
                  //      {
                  //         x = sNan;
                  //         y = sNan;
                  //         z = sNan;
                  //         grblReset();
                  //      }
                  //      else
                  //      {
                  //         x = ui->txtMPosX->text().toDouble();
                  //         y = ui->txtMPosY->text().toDouble();
                  //         z = ui->txtMPosZ->text().toDouble();
                  //      }
                  //      break;
                  //   }
                  //}
               }

               iFind = data.find("WPos:");
               // Update work coordinates
               //QRegExp wpx("WPos:([^,]*),([^,]*),([^,^>]*)");
               if (iFind >= 0
                     && (iFind = string_before(strX, data, ',', iFind + 5)) > 0
                     && (iFind = string_before(strY, data, ',', iFind + 1)) > 0
                     && (((string_before(strZ, data, ',', iFind + 1)) > 0)
                         || ((string_before(strZ, data, '>', iFind + 1)) > 0)))

               {
                  Application.m_straCoord[0] = strX;
                  Application.m_straCoord[1] = strY;
                  Application.m_straCoord[2] = strZ;
                  //QVector3D toolPosition;

                  //// Update tool position
                  //if (!(status == CHECK && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1))
                  //{
                  //   toolPosition = QVector3D(toMetric(ui->txtWPosX->text().toDouble()),
                  //                            toMetric(ui->txtWPosY->text().toDouble()),
                  //                            toMetric(ui->txtWPosZ->text().toDouble()));
                  //   m_toolDrawer.setToolPosition(m_codeDrawer->getIgnoreZ() ? QVector3D(toolPosition.x(), toolPosition.y(), 0) : toolPosition);
                  //}


                  //// toolpath shadowing
                  //if (m_processingFile && status != CHECK)
                  //{
                  //   GcodeViewParse *parser = m_currentDrawer->viewParser();

                  //   bool toolOntoolpath = false;

                  //   QList<int> drawnLines;
                  //   QList<LineSegment*> list = parser->getLineSegmentList();

                  //   for (int i = m_lastDrawnLineIndex; i < list.count()
                  //         && list.at(i)->getLineNumber()
                  //         <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt() + 1); i++)
                  //   {
                  //      if (list.at(i)->contains(toolPosition))
                  //      {
                  //         toolOntoolpath = true;
                  //         m_lastDrawnLineIndex = i;
                  //         break;
                  //      }
                  //      drawnLines << i;
                  //   }

                  //   if (toolOntoolpath)
                  //   {
                  //      foreach(int i, drawnLines)
                  //      {
                  //         list.at(i)->setDrawn(true);
                  //      }
                  //      if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
                  //   }
                  //   else if (m_lastDrawnLineIndex < list.count())
                  //   {
                  //      qDebug() << "tool missed:" << list.at(m_lastDrawnLineIndex)->getLineNumber()
                  //               << m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()
                  //               << m_fileProcessedCommandIndex;
                  //   }
                  //}
               }

            }
            else if (data.length() > 0)
            {

               synch_lock sl(mutex());

               if (m_commanda.length() > 0 && !dataIsFloating(data)
                     && (m_commanda[0].m_strCommand == "[CTRL+X]" || !dataIsReset(data)))
               {

                  if ((m_commanda[0].m_strCommand != "[CTRL+X]" && dataIsEnd(data))
                        || (m_commanda[0].m_strCommand == "[CTRL+X]" && dataIsReset(data)))
                  {

                     response.append(data);

                     command command = m_commanda.takeFirst();

                     sl.unlock();
                     //QTextBlock tb = ui->txtConsole->document()->findBlockByNumber(ca.consoleIndex);
                     //QTextCursor tc(tb);

                     //// Restore absolute/relative coordinate system after jog
                     //if (ca.command.toUpper() == "$G" && ca.tableIndex == -2)
                     //{
                     //   if (ui->chkKeyboardControl->isChecked()) m_absoluteCoordinates = response.contains("G90");
                     //   else if (response.contains("G90")) sendCommand("G90", -1, m_settings->showUICommands());
                     //}

                     //// Process parser status
                     //if (ca.command.toUpper() == "$G" && ca.tableIndex == -3)
                     //{
                     //   // Update status in visualizer window
                     //   ui->glwVisualizer->setParserStatus(response.left(response.indexOf("; ")));

                     //   // Store parser status
                     //   if (m_processingFile) storeParserState();

                     //   // Process spindle state
                     //   if (!response.contains("M5"))
                     //   {
                     //      m_spindleCW = response.contains("M3");
                     //      m_timerToolAnimation.start(25, this);
                     //      ui->cmdSpindle->setChecked(true);
                     //   }
                     //   else
                     //   {
                     //      m_timerToolAnimation.stop();
                     //      ui->cmdSpindle->setChecked(false);
                     //   }

                     //   // Spindle speed
                     //   QRegExp rx(".*S([\\d\\.]+)");
                     //   if (rx.indexIn(response) != -1)
                     //   {
                     //      double speed = toMetric(rx.cap(1).toDouble()); //RPM in imperial?
                     //      if (fabs(ui->txtSpindleSpeed->value() - speed) < 2.54) ui->txtSpindleSpeed->setStyleSheet("color: palette(text);");
                     //   }

                     //   // Feed
                     //   rx.setPattern(".*F([\\d\\.]+)");
                     //   if (rx.indexIn(response) != -1)
                     //   {
                     //      double feed = toMetric(rx.cap(1).toDouble());
                     //      double set = ui->chkFeedOverride->isChecked() ? m_originalFeed / 100 * ui->txtFeed->value()
                     //                   : m_originalFeed;
                     //      if (response.contains("G20")) set *= 25.4;
                     //      if (fabs(feed - set) < 2.54) ui->txtFeed->setStyleSheet("color: palette(text);");
                     //   }

                     //   m_updateParserStatus = true;
                     //}

                     //// Store origin
                     //if (ca.command == "$#" && ca.tableIndex == -2)
                     //{
                     //   qDebug() << "Received offsets:" << response;
                     //   QRegExp rx(".*G92:([^,]*),([^,]*),([^\\]]*)");

                     //   if (rx.indexIn(response) != -1)
                     //   {
                     //      if (m_settingZeroXY)
                     //      {
                     //         m_settingZeroXY = false;
                     //         m_storedX = toMetric(rx.cap(1).toDouble());
                     //         m_storedY = toMetric(rx.cap(2).toDouble());
                     //      }
                     //      else if (m_settingZeroZ)
                     //      {
                     //         m_settingZeroZ = false;
                     //         m_storedZ = toMetric(rx.cap(3).toDouble());
                     //      }
                     //      ui->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));
                     //   }
                     //}

                     //// Homing response
                     //if ((ca.command.toUpper() == "$H" || ca.command.toUpper() == "$T") && m_homing) m_homing = false;

                     // Reset complete
                     if (command.m_strCommand == "[CTRL+X]")
                     {

                        m_bResetCompleted = true;

                        m_bUpdateParserStatus = true;

                     }

                     //// Clear command buffer on "M2" & "M30" command (old firmwares)
                     //if ((ca.command.contains("M2") || ca.command.contains("M30")) && response.contains("ok") && !response.contains("[Pgm End]"))
                     //{
                     //   m_commands.clear();
                     //   m_queue.clear();
                     //}

                     //// Process probing on heightmap mode only from table commands
                     //if (ca.command.contains("G38.2") && m_heightMapMode && ca.tableIndex > -1)
                     //{
                     //   // Get probe Z coordinate
                     //   // "[PRB:0.000,0.000,0.000:0];ok"
                     //   QRegExp rx(".*PRB:([^,]*),([^,]*),([^]^:]*)");
                     //   double z = qQNaN();
                     //   if (rx.indexIn(response) != -1)
                     //   {
                     //      qDebug() << "probing coordinates:" << rx.cap(1) << rx.cap(2) << rx.cap(3);
                     //      z = toMetric(rx.cap(3).toDouble());
                     //   }

                     //   static double firstZ;
                     //   if (m_probeIndex == -1)
                     //   {
                     //      firstZ = z;
                     //      z = 0;
                     //   }
                     //   else
                     //   {
                     //      // Calculate delta Z
                     //      z -= firstZ;

                     //      // Calculate table indexes
                     //      int row = trunc(m_probeIndex / m_heightMapModel.columnCount());
                     //      int column = m_probeIndex - row * m_heightMapModel.columnCount();
                     //      if (row % 2) column = m_heightMapModel.columnCount() - 1 - column;

                     //      // Store Z in table
                     //      m_heightMapModel.setData(m_heightMapModel.index(row, column), z, Qt::UserRole);
                     //      ui->tblHeightMap->update(m_heightMapModel.index(m_heightMapModel.rowCount() - 1 - row, column));
                     //      updateHeightMapInterpolationDrawer();
                     //   }

                     //   m_probeIndex++;
                     //}

                     //// Change state query time on check mode on
                     //if (ca.command.contains(QRegExp("$[cC]")))
                     //{
                     //   m_timerStateQuery.setInterval(response.contains("Enable") ? 1000 : m_settings->queryStateTime());
                     //}

                     //// Add response to console
                     //if (tb.isValid() && tb.text() == ca.command)
                     //{

                     //   bool scrolledDown = ui->txtConsole->verticalScrollBar()->value() == ui->txtConsole->verticalScrollBar()->maximum();

                     //   // Update text block numbers
                     //   int blocksAdded = response.count("; ");

                     //   if (blocksAdded > 0) for (int i = 0; i < m_commands.count(); i++)
                     //      {
                     //         if (m_commands[i].consoleIndex != -1) m_commands[i].consoleIndex += blocksAdded;
                     //      }

                     //   tc.beginEditBlock();
                     //   tc.movePosition(QTextCursor::EndOfBlock);

                     //   tc.insertText(" < " + QString(response).replace("; ", "\r\n"));
                     //   tc.endEditBlock();

                     //   if (scrolledDown) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());
                     //}


                     // Add response to table, send next program commands
                     if (m_pcommandaFile != NULL)
                     {

                        // Only if command from table
                        if (command.m_iIndex >= 0)
                        {

                           synch_lock slList(Application.mutex());

                           Application.m_commanda[command.m_iIndex].m_estate = command::state_processed;

                           Application.m_commanda[command.m_iIndex].m_strResponse = response;

                           m_iFileProcessedCommandIndex = command.m_iIndex;

                           if (Application.m_plist != NULL)
                           {
                              Application.m_plist->_001EnsureVisible(command.m_iIndex);

                           }

                        }

                        //                     // Process error messages
                        //                     static bool holding = false;
                        //                     static QString errors;
                        //
                        //                     if (ca.tableIndex > -1 && response.toUpper().contains("ERROR") && !m_settings->ignoreErrors())
                        //                     {
                        //                        errors.append(QString::number(ca.tableIndex + 1) + ": " + ca.command
                        //                                      + " < " + response + "\n");
                        //
                        //                        m_senderErrorBox->setText(tr("Error message(s) received:\n") + errors);
                        //
                        //                        if (!holding)
                        //                        {
                        //                           holding = true;         // Hold transmit while messagebox is visible
                        //                           response.clear();
                        //
                        //                           m_serialPort.write("!");
                        //                           m_senderErrorBox->checkBox()->setChecked(false);
                        //                           qApp->beep();
                        //                           int result = m_senderErrorBox->exec();
                        //
                        //                           holding = false;
                        //                           errors.clear();
                        //                           if (m_senderErrorBox->checkBox()->isChecked()) m_settings->setIgnoreErrors(true);
                        //                           if (result == QMessageBox::Ignore) m_serialPort.write("~"); else on_cmdFileAbort_clicked();
                        //                        }
                        //                     }
                        //
                        // Check transfer complete (last row always blank, last command row = rowcount - 2)
                        if (isResetCommand(command.m_strCommand)
                              || m_iFileProcessedCommandIndex >= m_pcommandaFile->get_upper_bound())
                        {

                           m_bSend = false;

                           m_pcommandaFile = NULL;

                           fork([this]()
                           {

                              Application.on_transfer_success();

                           });

                        }
                        else
                        {

                           send_next_file_commands();

                        }

                     }

                     // Scroll to first line on "M30" command
                     //if (ca.command.contains("M30")) ui->tblProgram->setCurrentIndex(m_currentModel->index(0, 1));
                     if (command.m_strCommand.contains("M30"))
                     {

                        Application.m_plist->_001EnsureVisible(0);

                     }

                     // Toolpath shadowing on check mode
                     //if (m_statusCaptions.indexOf(ui->txtStatus->text()) == CHECK)
                     //{
                     //   GcodeViewParse *parser = m_currentDrawer->viewParser();
                     //   QList<LineSegment*> list = parser->getLineSegmentList();

                     //   if (!m_transferCompleted && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1)
                     //   {
                     //      int i;
                     //      QList<int> drawnLines;

                     //      for (i = m_lastDrawnLineIndex; i < list.count()
                     //            && list.at(i)->getLineNumber()
                     //            <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()); i++)
                     //      {
                     //         drawnLines << i;
                     //      }

                     //      if (!drawnLines.isEmpty() && (i < list.count()))
                     //      {
                     //         m_lastDrawnLineIndex = i;
                     //         QVector3D vec = list.at(i)->getEnd();
                     //         m_toolDrawer.setToolPosition(vec);
                     //      }

                     //      foreach(int i, drawnLines)
                     //      {
                     //         list.at(i)->setDrawn(true);
                     //      }
                     //      if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
                     //   }
                     //   else
                     //   {
                     //      foreach(LineSegment* s, list)
                     //      {
                     //         if (!qIsNaN(s->getEnd().length()))
                     //         {
                     //            m_toolDrawer.setToolPosition(s->getEnd());
                     //            break;
                     //         }
                     //      }
                     //   }
                     //}

                     response.clear();

                  }
                  else
                  {

                     response.append(data + "; ");

                  }

               }
               else
               {
                  // Unprocessed responses
                  // qDebug() << "floating response:" << data;

                  // Handle hardware reset
                  if (dataIsReset(data))
                  {
                     //qDebug() << "hardware reset";

                     //m_bTransferCompleted = true;

                     m_bResetting = false;
                     //m_bHoming = false;
                     m_iLastGrblStatus = -1;

                     m_bUpdateParserStatus = true;
                     m_bStatusReceived = true;

                     m_commanda.clear();
                     //m_queue.clear();

                     updateControlsState();
                  }
                  //ui->txtConsole->appendPlainText(data);
               }
            }
            else
            {
               // Blank response
               //            ui->txtConsole->appendPlainText(data);
            }
         }

      }
      catch (serial::SerialException * pe)
      {

         Application.m_strStatus = pe->what();

      }
      catch (serial::IOException * pe)
      {

         Application.m_strStatus = pe->what();

      }

   }



   bool control::send_command(string strCommand, int iLine)
   {

      if (m_pserial.is_null())
      {

         Application.m_strStatus = "Port not opened";

         return false;

      }

      strCommand.make_upper();

      class command command;

      command.m_strCommand = strCommand;

      command.m_iLength = strCommand.length() + 1;

      command.m_iIndex = iLine;

      {

         synch_lock sl(mutex());

         m_commanda.append(command);

      }

      if (m_pserial.is_null())
      {

         Application.m_strStatus = "Port not opened";

         return false;

      }

      try
      {

         synch_lock sl(&m_mutexSerial);

         m_pserial->write(strCommand + "\r");

      }
      catch (serial::SerialException * pe)
      {

         Application.m_strStatus = pe->what();

      }
      catch (serial::IOException * pe)
      {

         Application.m_strStatus = pe->what();

      }

      return true;

   }


   int control::bufferLength()
   {

      synch_lock sl(mutex());

      int length = 0;

      for (auto & ca : m_commanda)
      {

         length += ca.m_iLength;

      }

      return length;

   }


   void control::updateControlsState()
   {


   }


   void control::grblReset()
   {

      TRACE("grbl reset");

      try
      {

         synch_lock sl(&m_mutexSerial);

         u8 uch = (char)24;

         m_pserial->write(&uch, 1);

      }
      catch (serial::SerialException * pe)
      {

         Application.m_strStatus = pe->what();

         return;

      }
      catch (serial::IOException * pe)
      {

         Application.m_strStatus = pe->what();

         return;

      }
      //    m_serialPort.flush();

      //m_processingFile = false;
      //      m_bRun = false;
      //    m_transferCompleted = true;
      //  m_fileCommandIndex = 0;

      m_bResetting = true;
      //m_bHoming = false;
      m_bResetCompleted = false;
      //m_updateSpindleSpeed = true;
      m_iLastGrblStatus = -1;
      m_bStatusReceived = true;

      // Drop all remaining commands in buffer
      m_commanda.clear();
      //m_queue.clear();

      {

         Application.m_plist->reset_items();

      }

      // Prepare reset response catch
      command command;

      command.m_strCommand = "[CTRL+X]";

      //      if (m_settings->showUICommands()) ui->txtConsole->appendPlainText(ca.command);
      //ca.consoleIndex = m_settings->showUICommands() ? ui->txtConsole->blockCount() - 1 : -1;
      //command.consoleIndex = -1;

      command.m_iIndex = -1;

      command.m_iLength = command.m_strCommand.length() + 1;

      m_commanda.append(command);

      updateControlsState();

   }

   bool control::is_sending_file()
   {

      return m_bSend || Application.m_pcontrol->m_pcommandaFile != NULL;

   }


   void control::send_next_file_commands()
   {

      if (!m_bSend)
      {

         m_pcommandaFile = NULL;

         ::run_pred_on_main_thread([this]()
         {

            if (Application.m_emode == mode_library)
            {

               Application.m_pedit->enable_window(true);

               Application.m_pedit->SetFocus();

            }

            Application.m_pedit->_001SetText("", ::source_user);

         });

         return;

      }

      while (m_iFileCommandIndex < m_pcommandaFile->count())
      {

         command command;

         command = m_pcommandaFile->element_at(m_iFileCommandIndex);

         if (bufferLength() + command.length() + 1 >= m_iMaxBufferSize)
         {

            break;

         }

         m_pcommandaFile->element_at(m_iFileCommandIndex).m_estate = command::state_sent;

         send_command(command.m_strCommand, command.m_iIndex);

         m_iFileCommandIndex++;

      }

   }


   bool control::stop_sending_file()
   {

      if (!is_sending_file())
      {

         return false;

      }

      m_bSend = false;

      m_pcommandaFile = NULL;

      ::run_pred_on_main_thread([this]()
      {

         if (Application.m_emode == mode_library)
         {

            Application.m_pedit->enable_window(true);

            Application.m_pedit->SetFocus();

         }

         Application.m_pedit->_001SetText("", ::source_user);

      });

      return true;

   }


   bool control::start_sending_file(array < command > * pcommanda)
   {

      if (is_sending_file())
      {

         return false;

      }

      if (!Application.open_serial_port())
      {

         Application.m_strStatus = "Serial not connected";

         return false;

      }

      m_iFileProcessedCommandIndex = -1;

      m_iFileCommandIndex = 0;

      m_pcommandaFile = pcommanda;

      m_bSend = true;

      fork([this]()
      {

         send_next_file_commands();

      });

      return true;

   }


} // namespace grbl_merge_tool


