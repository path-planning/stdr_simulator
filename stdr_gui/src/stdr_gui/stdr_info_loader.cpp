/******************************************************************************
   STDR Simulator - Simple Two DImensional Robot Simulator
   Copyright (C) 2013 STDR Simulator
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
   
   Authors : 
   * Manos Tsardoulias, etsardou@gmail.com
   * Aris Thallas, aris.thallas@gmail.com
   * Chris Zalidis, zalidis@gmail.com 
******************************************************************************/

#include "stdr_gui/stdr_info_loader.h"

namespace stdr_gui{
	InfoLoader::InfoLoader(int argc, char **argv){
		this->argc=argc;
		this->argv=argv;
		
		setupUi(this);
		
		stdrInformationTree->setColumnCount(2);
		stdrInformationTree->setColumnWidth(0,200);
		
		generalInfo.setText(0,"Simulation information");
		robotsInfo.setText(0,"Robots");
		
		mapWidth.setText(0,"Map width");
		mapWidth.setText(1,"-");
		generalInfo.addChild(&mapWidth);
		mapHeight.setText(0,"Map height");
		mapHeight.setText(1,"-");
		generalInfo.addChild(&mapHeight);
		mapOcgd.setText(0,"Resolution");
		mapOcgd.setText(1,"-");
		generalInfo.addChild(&mapOcgd);
		
		stdrInformationTree->addTopLevelItem(&generalInfo);
		stdrInformationTree->addTopLevelItem(&robotsInfo);
		
		generalInfo.setExpanded(true);
		robotsInfo.setExpanded(true);
	}
	
	void InfoLoader::deleteTreeNode(QTreeWidgetItem *item){
		int count=item->childCount();
		for(int i=count-1;i>=0;i--)
			deleteTreeNode(item->child(i));
		delete item;
	}
	
	void InfoLoader::deleteTree(void){
		for(unsigned int i=0;i<robotsInfo.childCount();i++){
			deleteTreeNode(robotsInfo.child(i));
		}
	}
	
	void InfoLoader::updateMapInfo(float width,float height,float ocgd){
		mapWidth.setText(1,(QString().setNum(width)+QString(" m")));
		mapHeight.setText(1,(QString().setNum(height)+QString(" m")));
		mapOcgd.setText(1,(QString().setNum(ocgd)+QString(" m/px")));
	}
	
	void InfoLoader::updateRobots(const stdr_msgs::RobotIndexedVectorMsg& msg){
		for(unsigned int i=0;i<msg.robots.size();i++){
			QTreeWidgetItem	*rnode=new QTreeWidgetItem();
			rnode->setText(0,QString(msg.robots[i].name.c_str()));
			
			QTreeWidgetItem *radius=new QTreeWidgetItem();
			radius->setText(0,"Radius");
			radius->setText(1,(QString().setNum(msg.robots[i].robot.radius)+QString("m")));
			rnode->addChild(radius);
			
			QTreeWidgetItem *lasers=new QTreeWidgetItem(),
							*sonars=new QTreeWidgetItem(),
							*rfids=new QTreeWidgetItem(),
							*kinematics=new QTreeWidgetItem();

			lasers->setText(0,"Lasers");
			sonars->setText(0,"Sonars");
			rfids->setText(0,"RFID antennas");
			kinematics->setText(0,"Kinematic");
			
			for(unsigned int l=0;l<msg.robots[i].robot.laserSensors.size();l++){
				QTreeWidgetItem *lname;
				lname=new QTreeWidgetItem();
				lname->setText(0,msg.robots[i].robot.laserSensors[l].frame_id.c_str());

				QTreeWidgetItem *lrays=new QTreeWidgetItem();
				QTreeWidgetItem *lmaxrange=new QTreeWidgetItem();
				QTreeWidgetItem *lminrange=new QTreeWidgetItem();
				QTreeWidgetItem *lmaxangle=new QTreeWidgetItem();
				QTreeWidgetItem *lminangle=new QTreeWidgetItem();
				QTreeWidgetItem *lnoisemean=new QTreeWidgetItem();
				QTreeWidgetItem *lnoisestd=new QTreeWidgetItem();
				QTreeWidgetItem *lfreq=new QTreeWidgetItem();
				
				lrays->setText(0,"Rays");
				//~ lrays->setText(1,QString().setNum(msg.robots[i].robot.laserSensors[l].rays));
				lmaxrange->setText(0,"Max dist");
				lmaxrange->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].maxRange)+QString(" m")));
				lminrange->setText(0,"Min dist");
				lminrange->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].minRange)+QString(" m")));
				lmaxangle->setText(0,"Max angle");
				lmaxangle->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].maxAngle)+QString(" deg")));
				lminangle->setText(0,"Min angle");
				lminangle->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].minAngle)+QString(" deg")));
				lnoisemean->setText(0,"Noise (mean)");
				lnoisemean->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].noise.noiseMean)+QString(" m")));
				lnoisestd->setText(0,"Noise (std)");
				lnoisestd->setText(1,(QString().setNum(msg.robots[i].robot.laserSensors[l].noise.noiseStd)+QString(" m")));
									
				lname->addChild(lrays);
				lname->addChild(lmaxrange);
				lname->addChild(lminrange);
				lname->addChild(lmaxangle);
				lname->addChild(lminangle);
				lname->addChild(lnoisemean);
				lname->addChild(lnoisestd);
				
				lasers->addChild(lname);
			}
			
			rnode->addChild(lasers);
			rnode->addChild(sonars);
			rnode->addChild(rfids);
			rnode->addChild(kinematics);
			
			robotsInfo.addChild(rnode);
		}
	}
}
