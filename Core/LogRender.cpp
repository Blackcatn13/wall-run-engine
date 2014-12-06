#include "Core.h"
#include "Assert.h"
#include "LogRender.h"
#include <windows.h>
#include <vector>
#include "Utils/BaseUtils.h"
#include "ActionToInput.h"
#include "GraphicsManager.h"
#include "FontManager.h"

CLogRender::CLogRender()
: m_uIsFullFirstPage(false)
, m_uLinesPerPage(30)
, m_uCurrentBeginIndex(-1)
, m_uCurrentEndIndex(-1)
, m_uIsInLastLog(true)
, m_WindowsPos(10,14)
, m_bIsVisible(false)
, m_uLinesPerPageAnimation(1)
, m_bAnimation(false)
, m_fAnimationTime(0.f)
, m_fTotalAnimation(5.f)
, m_Quad2dColor(0.f,0.f,0.5f,0.7f)
, m_fAlpha(0.7f)
{
}

CLogRender::~CLogRender()
{

}


void CLogRender::SetLinePerPage (uint32 lines)					
{
	if (lines == 0)
	{
		lines = 1;
	}

	m_uLinesPerPage = lines;
	m_uCurrentBeginIndex = -1;
	m_uCurrentEndIndex = -1;
	m_uIsInLastLog = true;
}

void CLogRender::Update (float deltaTime)
{
	if (m_bAnimation)
	{
		float value;
		bool finish = m_LerpAnimator1D.Update(deltaTime,value);
		SetLinePerPage((uint32)(value));


		
		if( finish)
		{
			m_bAnimation = false;
			if (m_iDirecion == 1)
			{
				m_bIsVisible = true;
			}
			else if (m_iDirecion == -1)
			{
				m_bIsVisible = false;
			}
			m_uLinesPerPage = m_uLinesPerPageAnimation;
		}
	}
}

//-------Animation Functions------------//
void CLogRender::SetVisible (bool flag)
{
	//Mientras se esta realizando la animacion no permitimos tocar nada:
	if( !m_bAnimation)
	{
		if (m_bIsVisible == false && flag == true)
		{
			//Realizamos animación de aparecer:
			m_uLinesPerPageAnimation = m_uLinesPerPage;
			m_uLinesPerPage = 1;
			m_bIsVisible = true;
			m_bAnimation = true;
			m_LerpAnimator1D.SetValues(0.f,(float)m_uLinesPerPageAnimation, 0.8f,FUNC_INCREMENT);
			m_iDirecion = 1;
			
		}
		else if (m_bIsVisible == true && flag == false)
		{
			//Realizamos animación de desaparecer:
			m_bAnimation = true;
			m_iDirecion = -1;
			m_LerpAnimator1D.SetValues((float)m_uLinesPerPageAnimation,0.f, 0.8f,FUNC_INCREMENT);
			
		}
	}

}

void CLogRender::Render (CGraphicsManager* GraphicsManager, CFontManager* fm, CColor color)
{
	if( m_bIsVisible )
	{
		m_VecLogsToRender.clear();
		//Primero de todo hacemos un preproceso de los logs a pintar (si ocupa mas del ancho de pantalla ->salto de linea):
		CLogger* logger = CLogger::GetInstance();
		assert(m_uLinesPerPage <= logger->GetCapacity());
		const std::vector<SLog>& vecLogs = LOGGER->GetLogs();
		std::vector<SLog>::const_iterator it		= vecLogs.begin();
		std::vector<SLog>::const_iterator itEnd	= vecLogs.end();
		for (; it != itEnd; ++it)
		{
			SLog log = *it;
			uint32 sizeX = fm->SizeX(log.m_sLogText.c_str());
			uint32 w = sizeX + m_WindowsPos.x + INC_X_TO_MESSAGE;
			uint32 win_w, win_h;
			GraphicsManager->GetWidthAndHeight(win_w, win_h);
			if (w > win_w)
			{
				//Si ocupa mas de una linea hacemos un salto de linea:
				uint32 max_size = win_w -50 - m_WindowsPos.x - INC_X_TO_MESSAGE; //un margen de 50 por la izquierda
				float partitions = sizeX/(float)max_size;
				uint32 stringInc = uint32(log.m_sLogText.size()/partitions);

				std::string allText = log.m_sLogText;

				uint8 partitions_aux = uint8(partitions);
				for(uint8 i = 0; i < partitions_aux; i++)
				{
					SLog log_aux = log;
					if (log.m_eLogLevel == ELL_ERROR)
					{
						int jorls = 0;
					}
					if (i!=0)
					{
						log_aux.m_bLineFeed = true;
					}
					log_aux.m_eLogLevel = log.m_eLogLevel;
					log_aux.m_sLogText = allText.substr(0, stringInc);
					allText = allText.substr(stringInc);

					m_VecLogsToRender.push_back(log_aux);							
				}

				if (partitions - partitions_aux > 0)
				{
					SLog log_aux = log;
					log_aux.m_bLineFeed = true;
					log_aux.m_sLogText = allText;
					m_VecLogsToRender.push_back(log_aux);				
				}
				
			}
			else
			{
				m_VecLogsToRender.push_back(log);
			}
		}

		uint32 beginIndex, endIndex;
		ComputeBeginAndEnd(m_VecLogsToRender, beginIndex,endIndex);
		bool errors = LOGGER->Errors();
		bool warnings= LOGGER->Warnings();

		//Draw background quad2D
		uint32 w,h;
		GraphicsManager->GetWidthAndHeight(w,h);
		uint32 sizeY = fm->SizeY("|");
		uint32 h_aux = sizeY * (m_uLinesPerPage+4); //4 lienas que llenamos con informacion extra
		
		CColor edgeColor = colBLACK;
		edgeColor.SetAlpha(0.7f);
		GraphicsManager->DrawRectangle2D(Vect2i(5,2),w-10, h_aux,m_Quad2dColor,2,2,edgeColor);

		
		//Draw Info Text
		RenderLines (GraphicsManager, fm, m_VecLogsToRender, beginIndex, endIndex, errors, warnings, color );
		
	}
	else
	{
		std::string info, shortInfo;
		//CActionToInput* action2Input = CCore::GetInstance()->GetActionToInput();
		//action2Input->GetActionInfo(ACTION_LOGGER, info, shortInfo);

		std::string l_sInfo;
		baseUtils::FormatSrting(l_sInfo, "Press %s to view the Logger", shortInfo.c_str());

		//Draw background quad2D
		CColor quad2dColor(0.f,0.f,0.5f,0.7f);
		uint32 w = fm->SizeX(l_sInfo.c_str());
		uint32 h = fm->SizeY(l_sInfo.c_str());
		
		CColor edgeColor = colBLACK;
		edgeColor.SetAlpha(0.7f);
		GraphicsManager->DrawRectangle2D(Vect2i(m_WindowsPos.x, (uint32)(m_WindowsPos.y - h*0.5)), w, h, m_Quad2dColor, 1, 1, edgeColor);


		//Draw Info Text
		if	(LOGGER->Warnings())	color = colGREEN;
		if	(LOGGER->Errors())		color = colRED;
		

		fm->DrawDefaultText(m_WindowsPos.x,m_WindowsPos.y-10,color, l_sInfo.c_str());
	}

}

void CLogRender::NextLine()
{
	if( m_bAnimation || !m_bIsVisible) 
	{
		return;
	}

	if (m_uIsFullFirstPage)
	{
		//Obtenemos el logger:
		uint32 size = (uint32)m_VecLogsToRender.size();
		if(m_uCurrentEndIndex < size -1 )
		{
			m_uCurrentEndIndex++;
			m_uCurrentBeginIndex++;
		}
	}
}

void CLogRender::PrevLine()
{
	if( m_bAnimation || !m_bIsVisible)  
	{
		return;
	}

	if (m_uIsFullFirstPage)
	{
		//Obtenemos el logger:
		uint32 size = (uint32)m_VecLogsToRender.size();
		if(m_uCurrentBeginIndex > 0)
		{
			m_uCurrentEndIndex--;
			m_uCurrentBeginIndex--;
			m_uIsInLastLog = false;
		}
	}
}

void CLogRender::PageUp()
{
	if( m_bAnimation || !m_bIsVisible)  
	{
		return;
	}

	if (m_uIsFullFirstPage)
	{
		//Obtenemos el logger:
		unsigned int size = (uint32)m_VecLogsToRender.size();
		if (m_uCurrentBeginIndex+1 >= m_uLinesPerPage )
		{
			m_uCurrentEndIndex = m_uCurrentBeginIndex ;
			m_uCurrentBeginIndex -= m_uLinesPerPage -1;
		}
		else
		{
			m_uCurrentBeginIndex = 0;
			m_uCurrentEndIndex = m_uLinesPerPage -1;
		}

		if( m_uCurrentEndIndex != size -1)
		{
			m_uIsInLastLog = false;
		}
	}	
}

void CLogRender::PageDown()
{
	if( m_bAnimation || !m_bIsVisible)  
	{
		return;
	}

	if (m_uIsFullFirstPage && !m_uIsInLastLog)
	{
		//Obtenemos el logger:
		uint32 size = (uint32)m_VecLogsToRender.size();
		if(m_uCurrentEndIndex + m_uLinesPerPage -1< size )
		{
			m_uCurrentBeginIndex = m_uCurrentEndIndex;
			m_uCurrentEndIndex += m_uLinesPerPage -1;
		}
		else
		{
			m_uCurrentEndIndex = size-1;
			m_uCurrentBeginIndex = size -m_uLinesPerPage;
		}

		if( m_uCurrentEndIndex == size -1)
		{
			m_uIsInLastLog = true;
		}
	}
}

void CLogRender::ComputeBeginAndEnd ( const std::vector<SLog>& vecLogs, uint32& beginIndex, uint32& endIndex)
{
	uint32 sizeLogs = (uint32) vecLogs.size();
	if( sizeLogs == 0)
	{
		beginIndex = endIndex = 0;
		return;
	}

	if (!m_uIsFullFirstPage)
	{
		if( sizeLogs <= m_uLinesPerPage )	
		{
			beginIndex	= 0;
			endIndex = sizeLogs -1;
		}
		else
		{
			m_uIsFullFirstPage = true;
			beginIndex = sizeLogs -m_uLinesPerPage;
			endIndex = sizeLogs -1;
			m_uCurrentBeginIndex = beginIndex;
			m_uCurrentEndIndex = endIndex;
		}
	}
	else
	{
		if(m_uIsInLastLog)
		{
			if(sizeLogs < m_uLinesPerPage)
			     m_uLinesPerPage = sizeLogs;
			m_uCurrentBeginIndex = sizeLogs -m_uLinesPerPage;
			m_uCurrentEndIndex = sizeLogs -1;
		}
		beginIndex	=  m_uCurrentBeginIndex;
		endIndex	=  m_uCurrentEndIndex;
	}
}

void CLogRender::RenderLines (	CGraphicsManager* GraphicsManager, CFontManager* fm, const std::vector<SLog>& vecLogs, 
																uint32 beginIndex, uint32 endIndex, bool errors, bool warnings, CColor color)
{
	assert( beginIndex >= 0);
	assert( endIndex >= 0);
	assert( endIndex <= vecLogs.size() );
	assert( beginIndex <= vecLogs.size() );
	assert( beginIndex <= endIndex );
	assert( m_uLinesPerPage >= (endIndex- beginIndex) );

	RenderQuad();

	uint32 incY = 0;
	uint32 dy = 0;
	dy = m_WindowsPos.y;

	//Dibujamos la cabecera:
	std::string header = "	LINE   |         LEVEL         |                       MESSAGE                                          (Press F1 to hide the Logger)";
	incY += fm->DrawDefaultText(m_WindowsPos.x, dy, colWHITE, header.c_str());
	header = "______________________________________________________________________________________________________________________________________";
	dy +=fm->DrawDefaultText(m_WindowsPos.x, dy, colWHITE, header.c_str());

	uint32 lastLineInRender;
	uint32 totalLineInLogger;
	uint32 oldestLineSaved;

	uint32 linesToDraw;
	if( vecLogs.size() == 0 )
	{
		linesToDraw = 0;	
		lastLineInRender = 0;
		totalLineInLogger = 0;
		oldestLineSaved = 0;
	}
	else
	{
		linesToDraw = (endIndex- beginIndex)+1;
		lastLineInRender = vecLogs[endIndex].m_uLogLine;
		totalLineInLogger = vecLogs[vecLogs.size()-1].m_uLogLine;
		oldestLineSaved = vecLogs[0].m_uLogLine;
		++endIndex;
	}

	if (m_uLinesPerPage != linesToDraw)
	{
		//Aún no llenamos toda la ventana de logs, por lo tanto dejamos espacios en blanco al principio
		uint32 empty_lines = m_uLinesPerPage - linesToDraw;
		dy += incY * empty_lines; 
	}

	std::vector<SLog>::const_iterator it = vecLogs.begin()+beginIndex;
	std::vector<SLog>::const_iterator itEnd = vecLogs.begin()+endIndex;
	while( it != itEnd )
	{
		SLog log = *it;
		std::string level;
		CColor color = colWHITE;
		switch(log.m_eLogLevel)
		{
		case ELL_ERROR:		
			{
				color = colRED; //red
				level = "ERROR";
			}
			break;					
		case ELL_INFORMATION:
			{
				color = colWHITE; //white
				level = "INFORMATION";
			}
			break;
		case ELL_WARNING:
			{
				color = colGREEN; //green
				level = "WARNING";
			}
			break;
		case ELL_NONE :
			{
				color = colWHITE; //black
				level = "";
			}
			break;
		}

		//Dibujar por linea--> LINE  |  LEVEL  |  MESSAGE
		if (log.m_bLineFeed == false)
		{
			fm->DrawDefaultText(m_WindowsPos.x, dy, color, "%d",log.m_uLogLine);
			fm->DrawDefaultText(m_WindowsPos.x+INC_X_TO_LEVEL, dy, color, level.c_str());
		}
		dy +=fm->DrawDefaultText(m_WindowsPos.x+INC_X_TO_MESSAGE, dy,color, log.m_sLogText.c_str());
		it++;
	}

	color = colWHITE;
	header = "______________________________________________________________________________________________________________________________________";
	dy+=10;
	fm->DrawDefaultText(m_WindowsPos.x,dy-incY,color, header.c_str());
	//Vamos a pintar que linea estamos visualizando de las que hay lastLineInRender/totalLineInLogger
	fm->DrawDefaultText(	m_WindowsPos.x,dy,color, "		Line %d/%d     | Oldest record saved: line %d",
		lastLineInRender, totalLineInLogger, oldestLineSaved);

	if( errors )
	{
		color = colRED;
		fm->DrawDefaultText(	m_WindowsPos.x+300,dy,color, "|  Errors: YES  |");
	}
	else
	{
		color = colWHITE;
		fm->DrawDefaultText(	m_WindowsPos.x+300,dy,color, "|  Errors: NOT  |");
	}

	if( warnings )
	{
		color = colGREEN;
		fm->DrawDefaultText(	m_WindowsPos.x+400,dy,color, "Warnings: YES");
	}
	else
	{
		color = colWHITE;
		fm->DrawDefaultText(	m_WindowsPos.x+400,dy,color, "Warnings: NOT");
	}
	
	color = colWHITE;

	fm->DrawDefaultText(	m_WindowsPos.x+500,dy,color, "|      (Press PgDown, PgUp, Ctrl+Up or Ctrl+Down to view more logs)");
	header = "______________________________________________________________________________________________________________________________________";
	fm->DrawDefaultText(m_WindowsPos.x,dy,color, header.c_str());
}

void CLogRender::RenderQuad()
{
	//TODO.. cuando tengamos quad2d.. dibujaremos un rectangulo con cierta transparencia y cierto color. Para
	//distinguir los margenes del logrender.
}
