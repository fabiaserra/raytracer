#include "Hitables/BVHNode.h"

bool sortByAxisX(const std::shared_ptr<Hitable> & lhs, const std::shared_ptr<Hitable> & rhs)
{
	AABB boxLeft, boxRight;
	if (!lhs->boundingBox(0.f, 0.f, boxLeft) ||
		!rhs->boundingBox(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().x() < boxRight.min().x();
}

bool sortByAxisY(const std::shared_ptr<Hitable> & lhs, const std::shared_ptr<Hitable> & rhs)
{
	AABB boxLeft, boxRight;
	if (!lhs->boundingBox(0.f, 0.f, boxLeft) ||
		!rhs->boundingBox(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().y() < boxRight.min().y();
}

bool sortByAxisZ(const std::shared_ptr<Hitable> & lhs, const std::shared_ptr<Hitable> & rhs)
{
	AABB boxLeft, boxRight;
	if (!lhs->boundingBox(0.f, 0.f, boxLeft) ||
		!rhs->boundingBox(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().z() < boxRight.min().z();
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hitable>> hitables, float time0, float time1)
{
	// Sort hitable primitives based on the axis
	int axis = int(3.f * m_randomGenerator.nextFloat());
	if (axis == 0)
	{
		std::sort(hitables.begin(), hitables.end(), sortByAxisX);
	}
	else if (axis == 1)
	{
		std::sort(hitables.begin(), hitables.end(), sortByAxisY);
	}
	else
	{
		std::sort(hitables.begin(), hitables.end(), sortByAxisZ);
	}

	// Put half of the primitives in each subtree
	if (hitables.size() == 1)
	{
		m_left = m_right = hitables[0];
	}
	else if (hitables.size() == 2)
	{
		m_left = hitables[0];
		m_right = hitables[1];
	}
	else
	{
		size_t n = hitables.size();
		auto first = hitables.begin();
		auto last = hitables.end();

		std::vector<std::shared_ptr<Hitable>> leftHitables(first, first + n/2);
		m_left = std::make_shared<BVHNode>(leftHitables, time0, time1);

		std::vector<std::shared_ptr<Hitable>> rightHitables(first + n/2, last);
		m_right = std::make_shared<BVHNode>(rightHitables, time0, time1);
	}

	AABB boxLeft, boxRight;
	// We check whether there is a bounding box at all in case we
	// sent something like an infinite plane which doesn't have BBox
	if (!m_left->boundingBox(time0, time1, boxLeft) ||
		!m_right->boundingBox(time0, time1, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}
	m_box = AABB::surroundingBox(boxLeft, boxRight);
}

bool BVHNode::hit(const Ray & ray, float tMin, float tMax, HitRecord & record) const
{
	if (m_box.hit(ray, tMin, tMax))
	{
		HitRecord leftRecord;
		HitRecord rightRecord;
		
		// Check whether the box for the nodes are hit
		bool hitLeft = m_left->hit(ray, tMin, tMax, leftRecord);
		bool hitRight = m_right->hit(ray, tMin, tMax, rightRecord);

		// If they are hit, check the children
		if (hitLeft && hitRight)
		{
			if (leftRecord.t < rightRecord.t)
			{
				record = leftRecord;
			}
			else
			{
				record = rightRecord;
			}
			return true;
		}
		else if (hitLeft)
		{
			record = leftRecord;
			return true;
		}
		else if (hitRight)
		{
			record = rightRecord;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return false;
}

bool BVHNode::boundingBox(float time0, float time1, AABB & box) const
{
	box = m_box;
	return true;
}
