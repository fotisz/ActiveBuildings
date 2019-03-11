/**********************************************************************/
/***************** Copyright (C) 2014 Richard Spinney *****************/
/**********************************************************************/

/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#include "container.h"
#include "building.h"

#undef min
#undef max

void prepare_undo(void* root_){
    
    container* root = (container*) root_;
    
    root->old_building.points=root->current->points;
    root->old_building.adj_list=root->current->adj_list;
    root->old_building.desks=root->current->desks;
    root->old_building.kitchens=root->current->kitchens;
    root->old_building.male_toilets=root->current->male_toilets;
    root->old_building.female_toilets=root->current->female_toilets;
    root->old_building.printers=root->current->printers;
    root->old_building.meeting_rooms=root->current->meeting_rooms;
    root->old_building.exits=root->current->exits;
    root->old_building.stairs=root->current->stairs;
    root->old_building.lifts=root->current->lifts;
    root->old_building.walls_sitting=root->current->walls_sitting;
    root->old_building.walls_standing=root->current->walls_standing;
    root->old_building.floor_plans=root->current->floor_plans;
    root->old_building.floor_index=root->current->floor_index;
    root->old_building.vis_valid=root->current->vis_valid;
    root->old_building.map_valid=root->current->map_valid;
    root->undo_test=1;
}