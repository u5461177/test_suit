//
// Created by Ruiqi Zhang on 2019-05-28
//

#ifndef ATS_MSTDTASK_H
#define ATS_MSTDTASK_H

#include "tracktask.h"

namespace ats {

    class User;

    class MSTDTask : public TrackTask 
    {
    	public:
    		MSTDTask(std::shared_ptr<TaskInputData> task_input);
    		~MSTDTask() override = default;
    	
    	public:
    		Type get_type() const override;
    		void start(const nlohmann::json &params) override;

		protected:
			bool execution() override;
			int64_t calc_accumulative_quantity() override;

        int64_t get_total_quantity() override;
    };

	inline Type MSTDTask::get_type() const
	{
		return Type::MSTD;
	}

    inline int64_t MSTDTask::get_total_quantity() {
        return _quantity.first;
    }

}

#endif //ATS_MSTDTASK_H
