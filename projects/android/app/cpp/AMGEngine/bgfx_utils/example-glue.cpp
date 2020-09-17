/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../imgui/imgui.h"
#include "../entry/entry.h"
#include "../entry/cmd.h"
#include "../entry/dialog.h"
#include <bx/string.h>
#include <bx/timer.h>
#include <bx/math.h>

struct SampleData
{
	static constexpr uint32_t kNumSamples = 100;

	SampleData()
	{
		reset();
	}

	void reset()
	{
		m_offset = 0;
		bx::memSet(m_values, 0, sizeof(m_values) );

		m_min = 0.0f;
		m_max = 0.0f;
		m_avg = 0.0f;
	}

	void pushSample(float value)
	{
		m_values[m_offset] = value;
		m_offset = (m_offset+1) % kNumSamples;

		float min =  bx::kFloatMax;
		float max = -bx::kFloatMax;
		float avg =  0.0f;

		for (uint32_t ii = 0; ii < kNumSamples; ++ii)
		{
			const float val = m_values[ii];
			min  = bx::min(min, val);
			max  = bx::max(max, val);
			avg += val;
		}

		m_min = min;
		m_max = max;
		m_avg = avg / kNumSamples;
	}

	int32_t m_offset;
	float m_values[kNumSamples];

	float m_min;
	float m_max;
	float m_avg;
};

static SampleData s_frameTime;

static bool bar(float _width, float _maxWidth, float _height, const ImVec4& _color)
{
	const ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 hoveredColor(
		  _color.x + _color.x*0.1f
		, _color.y + _color.y*0.1f
		, _color.z + _color.z*0.1f
		, _color.w + _color.w*0.1f
		);

	ImGui::PushStyleColor(ImGuiCol_Button,        _color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  _color);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemSpacing.y) );

	bool itemHovered = false;

	ImGui::Button("", ImVec2(_width, _height) );
	itemHovered |= ImGui::IsItemHovered();

	ImGui::SameLine();
	ImGui::InvisibleButton("", ImVec2(bx::max(1.0f, _maxWidth-_width), _height) );
	itemHovered |= ImGui::IsItemHovered();

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

	return itemHovered;
}

static const ImVec4 s_resourceColor(0.5f, 0.5f, 0.5f, 1.0f);

static void resourceBar(const char* _name, const char* _tooltip, uint32_t _num, uint32_t _max, float _maxWidth, float _height)
{
	bool itemHovered = false;

	ImGui::Text("%s: %4d / %4d", _name, _num, _max);
	itemHovered |= ImGui::IsItemHovered();
	ImGui::SameLine();

	const float percentage = float(_num)/float(_max);

	itemHovered |= bar(bx::max(1.0f, percentage*_maxWidth), _maxWidth, _height, s_resourceColor);
	ImGui::SameLine();

	ImGui::Text("%5.2f%%", percentage*100.0f);

	if (itemHovered)
	{
		ImGui::SetTooltip("%s %5.2f%%"
			, _tooltip
			, percentage*100.0f
			);
	}
}

static bool s_showStats = false;

void showExampleDialog(entry::AppI* _app, const char* _errorText)
{
	char temp[1024];
	bx::snprintf(temp, BX_COUNTOF(temp), "Example: %s", _app->getName() );

	ImGui::SetNextWindowPos(
		  ImVec2(1000.0f, 0.0f)
		, ImGuiCond_FirstUseEver
		);
	ImGui::SetNextWindowSize(
		  ImVec2(200.0f, 45.0f)
		, ImGuiCond_FirstUseEver
		);

	ImGui::Begin(temp, NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SameLine();

	const bgfx::Stats* stats = bgfx::getStats();
	const double toMsCpu = 1000.0/stats->cpuTimerFreq;
	const double toMsGpu = 1000.0/stats->gpuTimerFreq;
	const double frameMs = double(stats->cpuTimeFrame)*toMsCpu;

	s_frameTime.pushSample(float(frameMs) );

	char frameTextOverlay[256];
	bx::snprintf(frameTextOverlay, BX_COUNTOF(frameTextOverlay), "%.1f FPS"
		, 1000.0f/s_frameTime.m_avg
		);

    ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImColor(0.0f, 0.5f, 0.15f, 1.0f).Value);
	ImGui::PlotLines(""
		, s_frameTime.m_values
		, SampleData::kNumSamples
		, s_frameTime.m_offset
		, frameTextOverlay
		, 0.0f
		, 60.0f
		, ImVec2(0.0f, 25.0f)
		);
	ImGui::PopStyleColor();


	ImGui::End();
}
